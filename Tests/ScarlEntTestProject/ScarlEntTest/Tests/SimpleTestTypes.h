#pragma once

struct Vec2
{
    int x = 0;
    int y = 0;

    bool operator==(const Vec2& other) const
    {
        return x == other.x && y == other.y;
    }
};

struct Vec3
{
    int x = 0;
    int y = 0;
    int z = 0;

    bool operator==(const Vec3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }
};
