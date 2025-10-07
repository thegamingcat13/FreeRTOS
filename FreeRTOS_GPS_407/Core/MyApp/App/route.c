#include <admin.h>
#include <stdbool.h>
#include "main.h"
#include "gps.h"
#include "cmsis_os.h"

osThreadId_t hWaypointTask;

struct waypoints
{
    float lon;
    float lat;
};

typedef struct waypoints sWaypoints;

sWaypoints Waypoints;

static int current = 0;
static bool filled = false;

sWaypoints aWaypoints[STRC_AMOUNT];

void Waypoint ()
{
    if (xSemaphoreTake(hGpsDataMutex, portMAX_DELAY) == pdTRUE)
    {
		if (parsed_gnrmc.status == 'A')
		{
			aWaypoints[current].lon = parsed_gnrmc.longitude;
			aWaypoints[current].lat = parsed_gnrmc.latitude;

			current++;
			if (current >= STRC_AMOUNT)
			{
				filled = true;
				current = 0;
			}
		}

		xSemaphoreGive(hGpsDataMutex);
    }
}

float returnWaypoints (int pointNumber, int type)
{
	switch (type)
	{
		case 1: //
				float lat;
				lat = aWaypoints[pointNumber].lat;
				return lat;
				break;

		case 2: //
				float lon;
				lon = aWaypoints[pointNumber].lon;
				return lon;
				break;

		default: //
				return 475.39182; //if we see this number in the terminal we know that something is verrrryyyyyy wrong. Should not happen :)
				break;
	}
}
