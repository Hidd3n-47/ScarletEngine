#pragma once

struct Int
{
    Int() = default;
    Int(const int x) : x(x) { }

    int x = 0;

    bool operator==(const Int& other) const
    {
        return x == other.x;
    }

    inline unordered_map<std::string, ScarlEnt::Property>* GetProperties() const { return nullptr; }
    inline static std::string ComponentTypeName() { return "Int"; }
    inline void SetEntityUniqueId(const Scarlet::Ulid id) { mEntityId = id; }
    inline Scarlet::Ulid GetEntityUniqueId() const { return mEntityId; }
private:
    Scarlet::Ulid mEntityId;
};

struct Char
{
    Char() = default;
    Char(const char x) : x(x) { }

    char x = 0;

    bool operator==(const Char& other) const
    {
        return x == other.x;
    }

    inline unordered_map<std::string, ScarlEnt::Property>* GetProperties() const { return nullptr; }
    inline static std::string ComponentTypeName() { return "Char"; }
    inline void SetEntityUniqueId(const Scarlet::Ulid id) { mEntityId = id; }
    inline Scarlet::Ulid GetEntityUniqueId() const { return mEntityId; }
private:
    Scarlet::Ulid mEntityId;
};

struct Vec2
{
    Vec2() = default;
    Vec2(const int x, const int y) : x(x), y(y) { }

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
    inline static std::string ComponentTypeName() { return "Vec2"; }
    inline void SetEntityUniqueId(const Scarlet::Ulid id) { mEntityId = id; }
    inline Scarlet::Ulid GetEntityUniqueId() const { return mEntityId; }
private:
    Scarlet::Ulid mEntityId;
};

struct Vec3
{
    Vec3() = default;
    Vec3(const int x, const int y, const int z) : x(x), y(y), z(z) {}

    int x = 0;
    int y = 0;
    int z = 0;

    bool operator==(const Vec3& other) const
    {
        return x == other.x && y == other.y && z == other.z;
    }

    inline unordered_map<std::string, ScarlEnt::Property>* GetProperties() const { return nullptr; }
    inline static std::string ComponentTypeName() { return "Vec3"; }
    inline void SetEntityUniqueId(const Scarlet::Ulid id) { mEntityId = id; }
    inline Scarlet::Ulid GetEntityUniqueId() const { return mEntityId; }
private:
    Scarlet::Ulid mEntityId;
};
