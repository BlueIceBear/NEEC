#ifndef _TEXTUAL_H_
#define _TEXTUAL_H_

#include "utilities.h"


void Textual(trip_node *, station_node *);

void MainMenu(int *);

void DataSelection();
void TripListing(trip_node *);
void StationListing();
void RouteListing();
void StatsListing();

void TimePeriod();
void WeekDay();
void TripDuration();

#endif