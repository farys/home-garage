#ifndef ButtonLogic_h
#define ButtonLogic_h

class ButtonLogic{
    public:
    void print(void *text){
        Serial.println((char*)text);
    }
};

#endif