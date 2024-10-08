#include "../include/Game.hpp"
#include "../include/MoveValidator.hpp"
#include "../include/Utility.hpp"

#include <sstream>

Game::Game(std::string fen) : Game(getFenTokens(fen))
{
}

Game::Game(std::vector<std::string> fenParts) : board(fenParts[0], fenParts[3])
{
	parseActiveColor(fenParts[1]);
	parseCastlingRights(fenParts[2]);
	parsehalfMoveClock(fenParts[4]);
	parseFullMoveNumber(fenParts[5]);
}

Color Game::getActiveColor() const
{
	return activeColor;
}

void Game::setActiveColor(Color activeColor)
{
	this->activeColor = activeColor;
}

Board &Game::getBoard()
{
	return board;
}

std::string Game::getFen()
{
	std::string fen = board.getFenPosition() + " ";
	fen += activeColor == Color::WHITE ? "w" : "b";
	fen += " ";
	if (whiteCastleRights.canCastleKingSide())
	{
		fen += "K";
	}
	if (whiteCastleRights.canCastleQueenSide())
	{
		fen += "Q";
	}
	if (blackCastleRights.canCastleKingSide())
	{
		fen += "k";
	}
	if (blackCastleRights.canCastleQueenSide())
	{
		fen += "q";
	}
	if (!blackCastleRights.canCastle() && !whiteCastleRights.canCastle())
	{
		fen += "-";
	}

	fen += " ";
	fen += board.getEnPassantTargetSquare().has_value() ? Utility::convertPositionToString(board.getEnPassantTargetSquare().value()) : "-";
	fen += " ";
	fen += std::to_string(halfMoveClock);
	fen += " ";
	fen += std::to_string(fullMoveNumber);

	return fen;
}

int Game::getHalfMoveClock() const
{
	return halfMoveClock;
}

void Game::setHalfMoveClock(int halfMoveClock)
{
	this->halfMoveClock = halfMoveClock;
}

int Game::getFullMoveNumber() const
{
	return fullMoveNumber;
}

void Game::setFullMoveNumber(int fullMoveNumber)
{
	this->fullMoveNumber = fullMoveNumber;
}

CastleRights Game::getWhiteCastleRights() const
{
	return whiteCastleRights;
}

CastleRights Game::getBlackCastleRights() const
{
	return blackCastleRights;
}

Move Game::getLastMove() const
{
	if (moveHistory.size() == 0)
	{
		throw std::invalid_argument("No moves have been made");
	}

	return moveHistory.back();
}

void Game::makeMove(Position from, Position to, PromotionPiece promotionPiece)
{
	if (!board.getPiece(from, activeColor).has_value())
	{
		throw std::invalid_argument("No piece at the from position");
	}

	// Validate move
	PieceType piece = board.getPiece(from, activeColor).value();
	MoveValidator::validateMove(from, to, piece, activeColor, *this);

	// Get move details
	std::optional<PieceType> capturedPiece = getCapturedPiece(piece, from, to);
	SpecialMove specialMove = getSpecialMove(piece, from, to, capturedPiece, promotionPiece);
	std::optional<Position> enPassantTargetSquare = board.getEnPassantTargetSquare();
	Move move = Move(from, to, piece, activeColor, capturedPiece, enPassantTargetSquare, specialMove, promotionPiece, whiteCastleRights, blackCastleRights, halfMoveClock, fullMoveNumber);
	addMoveToHistory(move);

	// Move piece
	board.movePiece(move);

	// Check if king is in check
	if (isInCheck())
	{
		unmakeMove();
		throw std::invalid_argument("Move puts king in check");
	}

	// Update castling rights
	updateCastlingRights(piece, activeColor, from);

	// Reset half move clock if a pawn is moved or a piece is captured
	if (capturedPiece.has_value() || piece == PieceType::PAWN)
	{
		halfMoveClock = 0;
	}
	else
	{
		halfMoveClock++;
	}

	// Increment full move number if black moves
	if (activeColor == Color::BLACK)
	{
		fullMoveNumber++;
	}

	hasCachedInCheckValue = false;
	switchActiveColor();
}

void Game::unmakeMove()
{
	if (moveHistory.size() == 0)
	{
		throw std::invalid_argument("No moves to undo");
	}

	// Get move details from history and remove move from history
	Move move = moveHistory.back();
	moveHistory.pop_back();

	// Undo board move details
	board.unmovePiece(move);

	// Undo castling rights
	blackCastleRights = move.getBlackCastleRights();
	whiteCastleRights = move.getWhiteCastleRights();

	// Undo half move clock and full move number
	halfMoveClock = move.getHalfMoveClock();
	fullMoveNumber = move.getFullMoveNumber();

	// Switch active color to the color that made the move
	activeColor = move.getColor();
	hasCachedInCheckValue = false;
}

uint64_t Game::perft(int depth)
{
	if (depth == 0)
	{
		return 1;
	}

	uint64_t nodes = 0;
	std::vector<Move> moves = generateLegalMoves();

	for (Move move : moves)
	{
		makeMove(move.getFrom(), move.getTo(), move.getPromotionPiece());
		nodes += perft(depth - 1);
		unmakeMove();
	}

	return nodes;
}

void Game::perftRoot(int depth, std::map<std::string, int> &output)
{
    uint64_t totalNodes = 0;
    std::vector<Move> moves = generateLegalMoves();

    for (Move move : moves)
    {
        makeMove(move.getFrom(), move.getTo(), move.getPromotionPiece());
        uint64_t nodes = perft(depth - 1);
        unmakeMove();

		std::string moveString = Utility::convertPositionToString(move.getFrom()) + Utility::convertPositionToString(move.getTo());

		output[moveString] = nodes;
        totalNodes += nodes;
    }

	output["Nodes"] = totalNodes;
}

std::vector<Move> Game::generateLegalMoves()
{
	std::vector<Move> moves;

	for (int square = 0; square < 64; square++)
	{
		// Get piece at square for active color
		Position from = Utility::calculatePosition(square);
		std::optional<PieceType> piece = board.getPiece(from, activeColor);
		if (!piece.has_value())
		{
			continue;
		}

		// Loop through potential moves
		Bitboard potentialMoves = MoveValidator::generatePotentialMoves(from, piece.value(), activeColor, board);
		while (potentialMoves.getValue())
		{
			// Get to square and remove from potential moves
			int toSquare = potentialMoves.bitScanForward();
			Position to = Utility::calculatePosition(toSquare);
			potentialMoves.clearBit(toSquare);

			// If move is invalid, continue
			if (!isValidMove(from, to, piece.value()))
			{
				continue;
			}

			// If move puts friendly king in check, continue
			// ! might need to be adjusted to account for promotion pieces
			if (movePutsKingInCheck(from, to, piece.value()))
			{
				continue;
			}

			// Get move details
			if (piece.value() == PieceType::PAWN && (to.row == 0 || to.row == 7))
			{
				for (PromotionPiece promotionPiece : {PromotionPiece::QUEEN, PromotionPiece::ROOK, PromotionPiece::BISHOP, PromotionPiece::KNIGHT})
				{
					Move move = composeMove(from, to, piece.value(), promotionPiece, whiteCastleRights, blackCastleRights, halfMoveClock, fullMoveNumber);
					moves.push_back(move);
				}
			}
			else
			{
				Move move = composeMove(from, to, piece.value(), PromotionPiece::NONE, whiteCastleRights, blackCastleRights, halfMoveClock, fullMoveNumber);
				moves.push_back(move);
			}
		}
	}

	return moves;
}

std::vector<std::string> Game::getFenTokens(std::string fen)
{
	std::vector<std::string> parts;
	std::istringstream iss(fen);
	std::string part;
	while (std::getline(iss, part, ' '))
	{
		parts.push_back(part);
	}

	return parts;
}

void Game::parseActiveColor(std::string color)
{
	if (color == "w")
	{
		activeColor = Color::WHITE;
	}
	else
	{
		activeColor = Color::BLACK;
	}
}

void Game::parsehalfMoveClock(std::string halfMoveClock)
{
	this->halfMoveClock = std::stoi(halfMoveClock);
}

void Game::parseFullMoveNumber(std::string fullMoveNumber)
{
	this->fullMoveNumber = std::stoi(fullMoveNumber);
}

void Game::parseCastlingRights(std::string castlingRights)
{
	bool whiteCanCastleKingside = castlingRights.find('K') != std::string::npos;
	bool whiteCanCastleQueenside = castlingRights.find('Q') != std::string::npos;
	bool blackCanCastleKingside = castlingRights.find('k') != std::string::npos;
	bool blackCanCastleQueenside = castlingRights.find('q') != std::string::npos;

	whiteCastleRights = CastleRights{whiteCanCastleKingside, whiteCanCastleQueenside};
	blackCastleRights = CastleRights{blackCanCastleKingside, blackCanCastleQueenside};
}

void Game::switchActiveColor()
{
	activeColor == Color::WHITE ? activeColor = Color::BLACK : activeColor = Color::WHITE;
}

void Game::incrementHalfMoveClock()
{
	halfMoveClock++;
}

void Game::incrementFullMoveNumber()
{
	fullMoveNumber++;
}

void Game::addMoveToHistory(Move move)
{
	moveHistory.push_back(move);
}

std::optional<PieceType> Game::getCapturedPiece(PieceType piece, Position from, Position to)
{
	Color opponentColor = (activeColor == Color::WHITE) ? Color::BLACK : Color::WHITE;

	// Regular capture
	if (board.getPiece(to, opponentColor).has_value())
	{
		return board.getPiece(to, opponentColor);
	}
	// En passant capture
	else if (piece == PieceType::PAWN && abs(from.row - to.row) == 1 && abs(from.col - to.col) == 1 && !board.getPiece(to, opponentColor).has_value())
	{
		return PieceType::PAWN;
	}
	// No capture
	else
	{
		return std::nullopt;
	}
}

std::optional<Position> Game::getEnPassantTargetSquare(PieceType piece, Position from, Position to, SpecialMove specialMove)
{
	if (specialMove == SpecialMove::DOUBLE_PAWN_PUSH)
	{
		return Position{(from.row + to.row) / 2, from.col};
	}
	else
	{
		return std::nullopt;
	}
}

SpecialMove Game::getSpecialMove(PieceType piece, Position from, Position to, std::optional<PieceType> capturedPiece, PromotionPiece promotionPiece)
{
	// Double Pawn Push
	if (piece == PieceType::PAWN && abs(from.row - to.row) == 2)
	{
		return SpecialMove::DOUBLE_PAWN_PUSH;
	}
	// En Passant
	else if (getBoard().getEnPassantTargetSquare().has_value() && capturedPiece.has_value() && capturedPiece.value() == PieceType::PAWN && piece == PieceType::PAWN && abs(from.col - to.col) == 1 && abs(from.row - to.row) == 1 && to == getBoard().getEnPassantTargetSquare().value())
	{
		return SpecialMove::EN_PASSANT;
	}
	// Castling
	else if (piece == PieceType::KING && abs(from.col - to.col) == 2 && from.row == to.row)
	{
		// Kingside Castle
		if (to.col == 6)
		{
			return SpecialMove::KINGSIDE_CASTLE;
		}
		// Queenside Castle
		else if (to.col == 2)
		{
			return SpecialMove::QUEENSIDE_CASTLE;
		}
	}
	// Promotion
	else if (promotionPiece != PromotionPiece::NONE)
	{
		return SpecialMove::PROMOTION;
	}
	// No special move

	return SpecialMove::NONE;
}

void Game::updateCastlingRights(PieceType piece, Color color, Position from)
{
	// Update castling rights if king moves from starting position
	if (piece == PieceType::KING && (from == Position{0, 4} || from == Position{7, 4}))
	{
		if (color == Color::WHITE)
		{
			whiteCastleRights.disable();
		}
		else
		{
			blackCastleRights.disable();
		}
	}

	// Update castling rights if rook moves from starting position
	if (piece == PieceType::ROOK && (from == Position{0, 0} || from == Position{0, 7} || from == Position{7, 0} || from == Position{7, 7}))
	{
		if (from.col == 0)
		{
			if (color == Color::WHITE)
			{
				whiteCastleRights.disableQueenSide();
			}
			else
			{
				blackCastleRights.disableQueenSide();
			}
		}
		else if (from.col == 7)
		{
			if (color == Color::WHITE)
			{
				whiteCastleRights.disableKingSide();
			}
			else
			{
				blackCastleRights.disableKingSide();
			}
		}
	}
}

bool Game::isInCheck()
{
	if (hasCachedInCheckValue)
	{
		return cachedInCheckValue;
	}

	cachedInCheckValue = MoveValidator::isSquareAttacked(getBoard(), getActiveColor(), getBoard().getKing(getActiveColor()));
	hasCachedInCheckValue = true;

	return cachedInCheckValue;
}

bool Game::isValidMove(Position from, Position to, PieceType piece)
{
	try
	{
		MoveValidator::validateMove(from, to, piece, activeColor, *this);
		return true;
	}
	catch(const std::exception& e)
	{
		return false;
	}
}

bool Game::movePutsKingInCheck(Position from, Position to, PieceType piece)
{
	try
	{
		makeMove(from, to, PromotionPiece::NONE);
		unmakeMove();
		return false;
	}
	catch(const std::exception& e)
	{
		return true;
	}
}

Move Game::composeMove(Position from, Position to, PieceType piece, PromotionPiece promotionPiece, CastleRights whiteCastleRights, CastleRights blackCastleRights, int halfMoveClock, int fullMoveNumber)
{
	std::optional<PieceType> capturedPiece = getCapturedPiece(piece, from, to);
	SpecialMove specialMove = getSpecialMove(piece, from, to, capturedPiece, PromotionPiece::NONE);
	std::optional<Position> enPassantTargetSquare = board.getEnPassantTargetSquare();

	return Move(from, to, piece, activeColor, capturedPiece, enPassantTargetSquare, specialMove, promotionPiece, whiteCastleRights, blackCastleRights, halfMoveClock, fullMoveNumber);
}