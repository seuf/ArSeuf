#ifndef __MELODY_H__
#define __MELODY_H__

class Melody {
  public:
    Melody();
    void attach();
    void playMelody();
    void playMelody2();
  
  private:
    void playTone(int tone, int duration);
    void playNote(char note, int duration);
	
};

#endif
