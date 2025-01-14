/***************************************************************************
                          boardsetup.h  -  Board setup dialog
                             -------------------
    begin                : Mon 14 Nov 2005
    copyright            : (C) 2005 Michal Rudolf <mrudolf@kdewebdev.org>
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BOARDSETUPDIALOG_H_INCLUDED
#define BOARDSETUPDIALOG_H_INCLUDED

#include "ui_boardsetup.h"
#include "board.h"

using namespace chessx;

class QMenu;

/**  @ingroup GUI
  The BoardSetupDialog class represents a dialog for setting up a position.  */

class BoardSetupDialog : public QDialog
{
    Q_OBJECT
public:
    /** Create ChessX Preferences dialog */
    BoardSetupDialog(QWidget* parent = nullptr, Qt::WindowFlags f = nullptr);
    /** Destroy the dialog */
    virtual ~BoardSetupDialog();
    /** Set current board */
    void setBoard(const BoardX& b);
    /** Set current board */
    void setFlipped(bool flipped);
    /** Set current board */
    bool isFlipped() const;
    /** Return current board */
    BoardX board() const;

protected:
    /** Resize board upon showing it the first time */
    virtual void showEvent(QShowEvent *e);
    /** Scroll current piece with a wheel */
    virtual void wheelEvent(QWheelEvent* e);

protected:
    /** Display side to move */
    void showSideToMove();
    /** Message for incorrect setups. Returns empty string if the position is correct. */
    QString boardStatusMessage() const;
    /** Sets status message for the board (either fen or error message). */
    void setStatusMessage();

    char castlingFile00() const;
    char castlingFile000() const;
public slots:
    /** Start a drag operation */
    void startDrag(QWidget* w, QMouseEvent* event);
    /** A Piece was clicked */
    void labelClicked(Piece p);
    void mirrorVertical();
    void mirrorHorizontal();
    void swapColors();

signals:
    /** Request to clear the background of a piece label */
    void signalClearBackground(Piece p);

protected slots:
    void restoreLayout();
    void chess960posChanged(int value);
    void chess960randomPos();
private slots:
    /** If the position is legal, accept it and close the dialog. */
    void accept();
    /** Dialog closed otherwise */
    void reject();
    /** Clear board. */
    void slotClear();
    /** Restore standard position */
    void slotReset();
    /** Copy current position as FEN. */
    void slotCopyFen();
    /** Copy current position as text. */
    void slotCopyText();
    /** Paste FEN from clipboard as current position. */
    void slotPasteFen();
    /** Change current piece using wheel */
    void slotChangePiece(int dir);
    /** A Drop operation was inidcated by the boardview */
    void slotDroppedPiece(chessx::Square s, Piece p);
    /** Select square and insert piece */
    void slotSelected(chessx::Square s, int button);
    /** Manually adjust castling rights. */
    void slotCastlingRights();
    /** Manually adjust en passant square. */
    void slotEnPassantSquare();
    /** Select Chess960. */
    void slotChess960();
    /** Adjust halfmove clock. */
    void slotHalfmoveClock();
    /** Adjust move number. */
    void slotMoveNumber();
    /** Move piece */
    void slotMovePiece(chessx::Square from, chessx::Square to);
    /** Copy piece */
    void slotCopyPiece(chessx::Square from, chessx::Square to);
    /** Remove piece */
    void slotInvalidMove(chessx::Square from);
    /** Toggle side to move */
    void slotToggleSide();

private:
    Ui::BoardSetupDialog ui;
    QPushButton *copyButton;
    QPushButton *btCopyText;

    Color m_toMove;
    int m_wheelCurrentDelta;
    int m_minDeltaWheel;
    Piece m_selectedPiece;
    bool inDrag;
};

#endif
