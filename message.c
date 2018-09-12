#include "message.h"

void addCharToArray(char c)
{

	struct myChar new;
	new.crop.w = FONT_W;
	new.crop.h = FONT_H;
	new.crop.y = 0;

	new.pos.x = messageSize * (FONT_W + 4) + 96;
	new.pos.y = SCREEN_H - 110;
	new.pos.w = FONT_W;
	new.pos.h = FONT_H;

	int x = 0;

	char alpha[] = {'a','b','c','d','e','f','g','h','i','j','k','l','m','n',
				'o','p','q','r','s','t','u','v','w','x','y','z',' ','?',':','!'};

	
	for (int i = 0; i < ALPHA_SIZE; i++) {
		if (c == alpha[i]) {
			x = i;
		}
	}
	
	new.crop.x = (FONT_W * x);

	messageArray[messageSize] = new;
	messageSize++;
}

void showMsg(char* message, int messLen)
{
	showMessage = 1;
	messageSize = 0;
	messageArray = malloc(sizeof(struct myChar) * messLen);
	
	if (messageArray == NULL) {
		printf("init messageArray mem alloc failed\n");
	}
	
	for (int i = 0; i < messLen; i++) {
		addCharToArray(message[i]);
	}
	

}

void stopShowMsg()
{
	free(messageArray);
	messageArray = NULL;
	showMessage = 0;
}
