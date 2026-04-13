#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

//头文件包含
extern "C"
{
	#include "user_app.h"
}


Model::Model() : modelListener(0)
{
	
}

void Model::tick()
{
	
}
void Model::ipStrToInt(uint8_t* sip, uint8_t* nip)
{
    char num[4];
    int numIndex = 0;
    int nipIndex = 0;
    while(1)
    {
        if(*sip=='.' || *sip=='\0')
        {
            num[numIndex] = '\0';
            numIndex = 0;
            nip[nipIndex++] = atoi(num);
        }
        else
        {
            num[numIndex++] = *sip;
        }
        if(*sip == '\0')
            break;
        sip++;
    }
}

