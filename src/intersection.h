#pragma once

#include "Solid.h"

void intersect_solids(std::vector<Solid>& solids);

void intersect(Solid& home_solid, Solid& near_solid);

std::tuple<bool, Face, Face> face_intersect(Face& home_face, Face& near_face);

void draw_new_surfaces(Solid& home_solid, Solid& near_solid);