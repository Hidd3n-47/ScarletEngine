#pragma once

struct Int
{
    int x = 0;

    bool operator==(const Int& other) const
    {
        return x == other.x;
    }

    inline unordered_map<std::string, ScarlEnt::Property>* GetProperties() const { return nullptr; }
};

struct Vec2
{
    int x = 0;
    int y = 0;

    Vec2 operator+=(const Vec2& other)
    {
        x += other.x;
        y += other.y;

        return *this;
    }

    bool operator==(const Vec2& other) const
    {
        return x == other.x && y == other.y;
    }

    inline unordered_map<std::string, ScarlEnt::Property>* GetProperties() const { return nullptr; }
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

    inline unordered_map<std::string, ScarlEnt::Property>* GetProperties() const { return nullptr; }
};
