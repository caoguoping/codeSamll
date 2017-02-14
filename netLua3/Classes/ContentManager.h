
#include "cocos2d.h"
using namespace cocos2d;

class ContentManager : public cocos2d::Ref
{
public:
    static ContentManager *getInstance();
	ContentManager();
	void test();
    
};

