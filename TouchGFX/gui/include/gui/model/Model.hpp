#ifndef MODEL_HPP
#define MODEL_HPP

//头文件包含
extern "C"
{
	#include "user_app.h"
}

class ModelListener;


class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

	void tick();
  void ipStrToInt(uint8_t* sip, uint8_t* nip); 
protected:
    ModelListener* modelListener;
};

#endif // MODEL_HPP
