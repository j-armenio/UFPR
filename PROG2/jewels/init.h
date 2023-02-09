#ifndef __INIT_H__
#define __INIT_H__

#define DISPLAY_HEIGHT 900
#define DISPLAY_WIDTH 1600

void mustInit(bool test, const char *description);
void initAll(ALLEGRO_DISPLAY *disp, ALLEGRO_EVENT_QUEUE *evQueue);

#endif