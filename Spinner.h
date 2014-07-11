#ifndef Spinner_h
#define Spinner_h

struct Pos
{
  class Pos* Next;
  class Pos* Last;
  int Value;
};

class Spinner
{
public:
  Spinner();
  ~Spinner();
  
  int Increment();
  int Decrement();

private:
  Pos Pos1;
  Pos Pos2;
  Pos Pos3;
  Pos Pos4;
  Pos Pos5;
  Pos Pos6;
  Pos Pos7;
  Pos Pos8;
  Pos Pos9;
  Pos Pos0;
  Pos* currentPos;

  void InitPos(Pos &mpos, Pos* next, Pos* last, int value);
};

#endif





