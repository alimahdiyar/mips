#include <iostream>
using namespace std;
class Register{
public:
    bool enable= true; //value jadid biyad dakhel reg ya na

    Register(){
        this->value = 0;
    }

    Register(long value){
        Register::value = value;
    }

    long getValue() const {
        return value;
    }

    void setValue(long newValue) {
        Register::newValue = newValue;
    }
    void regClockcycle(){
        if(enable){
            if(newValue != -1){
                value=newValue;
                newValue=-1;
            } else{
                enable=true;
            }
        }



    }
private:
    long value;// meghdar register
protected:
    long newValue=-1;//value jadid ke mikhad rekhte beshe dakhel reg
};

int main() {
    Register a(5);
    Register b(6);
    cout << a.getValue();
    return 0;
}