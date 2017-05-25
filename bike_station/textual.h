#ifndef _TEXTUAL_H_
#define _TEXTUAL_H_

#include "utilities.h"


void Textual(trip_node *, station_node *, int, int, int);

void MainMenu(int *);

void DataSelection(trip_node **, station_node **, trip_node *, station_node *, int *);
void TripListing(trip_node *);
void StationListing(trip_node *, station_node *, int, int);
void RouteListing();
void StatsListing();

void TimePeriod(trip_node **, station_node **, int, int, int, int, int);
void WeekDay(trip_node **, int, int);
void TripDuration(trip_node **, int, int);
void RemoveSelections(trip_node **, trip_node *);

void GenderStats();
void AgeStats();
void DurationStats();
void VelocityStats();

#endif