#ifndef POINT_LIGHTS_BUFFER_H
#define POINT_LIGHTS_BUFFER_H

#include <vector>

struct PointLightData {
	std::vector<float> mData;
	bool operator==(const PointLightData&) const = default;
};

#endif