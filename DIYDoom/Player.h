#pragma once

// a "Player" is a "Things" in the WAD file
class Player
{
public:
    Player(int ID) :
        m_playerID(ID),
        m_xPosition(0),
        m_yPosition(0),
        m_direction(0) {}
    ~Player() {}

    void SetXPosition(int xPosition) { m_xPosition = xPosition; }
    void SetYPosition(int yPosition) { m_yPosition = yPosition; }
    void SetDirection(int direction) { m_direction = direction; }

    int GetID() const { return m_playerID; }
    int GetXPosition() const { return m_xPosition; }
    int GetYPosition() const { return m_yPosition; }
    int GetDirection() const { return m_direction; }

protected:
    int m_playerID;
    int m_xPosition;
    int m_yPosition;
    int m_direction;

};

