#include "Spinner.h"

Spinner::Spinner()
{
  InitPos(Pos1, &Pos2, &Pos0, 1);
  InitPos(Pos2, &Pos3, &Pos1, 2);
  InitPos(Pos3, &Pos4, &Pos2, 3);
  InitPos(Pos4, &Pos5, &Pos3, 4);
  InitPos(Pos5, &Pos6, &Pos4, 5);
  InitPos(Pos6, &Pos7, &Pos5, 6);
  InitPos(Pos7, &Pos8, &Pos6, 7);
  InitPos(Pos8, &Pos9, &Pos7, 8);
  InitPos(Pos9, &Pos0, &Pos8, 9);
  InitPos(Pos0, &Pos1, &Pos9, 0);
  currentPos = &Pos1;
}

Spinner::~Spinner()
{
}

int Spinner::Increment()
{
  currentPos = currentPos->Next;
    return currentPos->Value;
}

int Spinner::Decrement()
{
  currentPos = currentPos->Last;
    return currentPos->Value;
}

void Spinner::InitPos(Pos &mpos, Pos* next, Pos* last, int value)
{
  mpos.Next = next;
  mpos.Last = last;
  mpos.Value = value;
}

