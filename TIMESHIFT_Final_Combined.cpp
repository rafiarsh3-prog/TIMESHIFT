#include <GL/glut.h>
#include <cmath>
#include <iostream>

using namespace std;

struct Platform
{
    float x1, x2, y, h;
};

float tx = 50.0f;
float ty = 80.0f;

float velocityX = 0.0f;
float velocityY = 0.0f;

float gravity = -0.65f;
float jumpPower = 15.0f;

float moveSpeed = 5.0f;
float acceleration = 0.8f;
float friction = 0.75f;

int level = 1;
int timeline = 0;

bool onGround = false;
bool keyDown[256] = {false};

bool crystal1 = false;
bool crystal2 = false;
bool crystal3 = false;

bool switch1 = false;
bool switch2 = false;
bool switch3 = false;

float angle1 = 0.0f;
float angle2 = 0.0f;
float angle3 = 0.0f;

bool gameComplete = false;

Platform platforms[20];
int platformCount = 0;

// [OBJ-09] Circle 
void drawCircle(float cx, float cy, float r)
{
    glBegin(GL_POLYGON);
    for(int i = 0; i < 100; i++)
    {
        float a = 2.0f * 3.14159f * i / 100;
        float x = cx + r * cos(a);
        float y = cy + r * sin(a);
        glVertex3f(x, y, 0.0f);
    }
    glEnd();
}

// [OBJ-08] DDA Line Platform
void drawDDA(float x1, float y1, float x2, float y2)
{
    float dx = x2 - x1;
    float dy = y2 - y1;
    float m = dy / dx;
    float x = x1;
    float y = y1;

    glPointSize(3);
    glBegin(GL_POINTS);

    if(fabs(m) < 1)
    {
        while(x <= x2 && y <= y2)
        {
            glVertex2i(x, y);
            x = x + 1;
            y = y + m;
        }
    }
    else
    {
        while(y <= y2)
        {
            glVertex2i(x, y);
            y = y + 1;
            x = x + (1 / m);
        }
    }
    glEnd();
}

// [OBJ-02] Platform 
void storePlatform(float x1, float x2, float y, float h)
{
    platforms[platformCount].x1 = x1;
    platforms[platformCount].x2 = x2;
    platforms[platformCount].y = y;
    platforms[platformCount].h = h;
    platformCount++;
}

// [OBJ-01] Player / Character
void drawPlayer()
{
    glColor3f(1.0f, 0.2f, 0.2f);
    glBegin(GL_QUADS);
    glVertex3f(tx, ty, 0.0f);
    glVertex3f(tx + 30, ty, 0.0f);
    glVertex3f(tx + 30, ty + 40, 0.0f);
    glVertex3f(tx, ty + 40, 0.0f);
    glEnd();

    glColor3f(1.0f, 0.8f, 0.5f);
    drawCircle(tx + 15, ty + 50, 10);
}

// [OBJ-03] Portal
void drawPortal(float x, float y)
{
    glColor3f(0.7f, 0.2f, 1.0f);
    drawCircle(x, y, 30);

    glColor3f(0.1f, 0.0f, 0.2f);
    drawCircle(x, y, 18);
}

// [OBJ-04] Crystal
void drawCrystal(float x, float y)
{
    glColor3f(1.0f, 0.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex3f(x, y + 15, 0.0f);
    glVertex3f(x + 12, y, 0.0f);
    glVertex3f(x + 24, y + 15, 0.0f);
    glVertex3f(x + 12, y + 30, 0.0f);
    glEnd();
}

// [OBJ-05] Rotating Cube 
void drawCube(float x, float y, float angle, bool colorful)
{
    glLoadIdentity();

    glTranslatef(x, y, 0.0f);
    glRotatef(angle, 1.0f, 1.0f, 0.0f);
    glScalef(20.0f, 20.0f, 20.0f);

    if(colorful)
        glColor3f(1.0f, 0.0f, 0.0f);
    else
        glColor3f(0.7f, 0.7f, 0.7f);

    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glEnd();

    if(colorful)
        glColor3f(0.0f, 1.0f, 0.0f);
    else
        glColor3f(0.7f, 0.7f, 0.7f);

    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glEnd();

    if(colorful)
        glColor3f(0.0f, 0.0f, 1.0f);
    else
        glColor3f(0.7f, 0.7f, 0.7f);

    glBegin(GL_QUADS);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f( 1.0f, 1.0f, -1.0f);
    glVertex3f( 1.0f, 1.0f,  1.0f);
    glVertex3f(-1.0f, 1.0f,  1.0f);
    glEnd();

    if(colorful)
        glColor3f(1.0f, 1.0f, 0.0f);
    else
        glColor3f(0.7f, 0.7f, 0.7f);

    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glEnd();

    if(colorful)
        glColor3f(1.0f, 0.0f, 1.0f);
    else
        glColor3f(0.7f, 0.7f, 0.7f);

    glBegin(GL_QUADS);
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f, -1.0f);
    glVertex3f(1.0f,  1.0f,  1.0f);
    glVertex3f(1.0f, -1.0f,  1.0f);
    glEnd();

    if(colorful)
        glColor3f(0.0f, 1.0f, 1.0f);
    else
        glColor3f(0.7f, 0.7f, 0.7f);

    glBegin(GL_QUADS);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glEnd();

    glutWireCube(2.0f);
}

// [OBJ-06] Timeline Level 1
void level1()
{
    platformCount = 0;

    if(timeline == 0)
    {
        glColor3f(0.2f, 0.7f, 0.3f);

        glBegin(GL_QUADS);
        glVertex3f(0, 50, 0);
        glVertex3f(250, 50, 0);
        glVertex3f(250, 80, 0);
        glVertex3f(0, 80, 0);
        glEnd();
        storePlatform(0, 250, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(650, 50, 0);
        glVertex3f(900, 50, 0);
        glVertex3f(900, 80, 0);
        glVertex3f(650, 80, 0);
        glEnd();
        storePlatform(650, 900, 50, 30);

        glColor3f(1.0f, 1.0f, 1.0f);
    }
    else
    {
        glColor3f(0.2f, 0.5f, 0.9f);
        glBegin(GL_QUADS);
        glVertex3f(0, 50, 0);
        glVertex3f(900, 50, 0);
        glVertex3f(900, 80, 0);
        glVertex3f(0, 80, 0);
        glEnd();
        storePlatform(0, 900, 50, 30);
    }

    drawPortal(820, 110);
}

// [OBJ-06] Timeline Level 2
void level2()
{
    platformCount = 0;

    if(timeline == 0)
    {
        glColor3f(0.2f, 0.7f, 0.3f);

        glBegin(GL_QUADS);
        glVertex3f(0, 50, 0);
        glVertex3f(300, 50, 0);
        glVertex3f(300, 80, 0);
        glVertex3f(0, 80, 0);
        glEnd();
        storePlatform(0, 300, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(300, 50, 0);
        glVertex3f(600, 50, 0);
        glVertex3f(600, 80, 0);
        glVertex3f(300, 80, 0);
        glEnd();
        storePlatform(300, 600, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(600, 50, 0);
        glVertex3f(900, 50, 0);
        glVertex3f(900, 80, 0);
        glVertex3f(600, 80, 0);
        glEnd();
        storePlatform(600, 900, 50, 30);
    }
    else
    {
        glColor3f(0.3f, 0.5f, 0.9f);

        glBegin(GL_QUADS);
        glVertex3f(0, 50, 0);
        glVertex3f(300, 50, 0);
        glVertex3f(300, 80, 0);
        glVertex3f(0, 80, 0);
        glEnd();
        storePlatform(0, 300, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(600, 50, 0);
        glVertex3f(900, 50, 0);
        glVertex3f(900, 80, 0);
        glVertex3f(600, 80, 0);
        glEnd();
        storePlatform(600, 900, 50, 30);
    }

    drawPortal(820, 110);
}

// [OBJ-06] Timeline-Level 3
void level3()
{
    platformCount = 0;

    if(timeline == 0)
    {
        glColor3f(0.2f, 0.7f, 0.3f);

        glBegin(GL_QUADS);
        glVertex3f(0, 50, 0);
        glVertex3f(220, 50, 0);
        glVertex3f(220, 80, 0);
        glVertex3f(0, 80, 0);
        glEnd();
        storePlatform(0, 220, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(180, 130, 0);
        glVertex3f(340, 130, 0);
        glVertex3f(340, 160, 0);
        glVertex3f(180, 160, 0);
        glEnd();
        storePlatform(180, 340, 130, 30);

        glColor3f(0.2f, 0.7f, 0.3f);
        drawDDA(460, 290, 620, 290);
    }
    else
    {
        glColor3f(0.3f, 0.5f, 0.9f);

        glBegin(GL_QUADS);
        glVertex3f(0, 50, 0);
        glVertex3f(300, 50, 0);
        glVertex3f(300, 80, 0);
        glVertex3f(0, 80, 0);
        glEnd();
        storePlatform(0, 300, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(300, 180, 0);
        glVertex3f(480, 180, 0);
        glVertex3f(480, 210, 0);
        glVertex3f(300, 210, 0);
        glEnd();
        storePlatform(300, 480, 180, 30);

        glBegin(GL_QUADS);
        glVertex3f(500, 300, 0);
        glVertex3f(660, 300, 0);
        glVertex3f(660, 330, 0);
        glVertex3f(500, 330, 0);
        glEnd();
        storePlatform(500, 660, 300, 30);

        glBegin(GL_QUADS);
        glVertex3f(650, 420, 0);
        glVertex3f(900, 420, 0);
        glVertex3f(900, 450, 0);
        glVertex3f(650, 450, 0);
        glEnd();
        storePlatform(650, 900, 420, 30);
    }

    drawPortal(800, 480);
}

// [OBJ-06] Timeline - Level 4
void level4()
{
    platformCount = 0;

    if(timeline == 0)
    {
        glColor3f(0.2f, 0.7f, 0.3f);

        glBegin(GL_QUADS);
        glVertex3f(0, 50, 0);
        glVertex3f(250, 50, 0);
        glVertex3f(250, 80, 0);
        glVertex3f(0, 80, 0);
        glEnd();
        storePlatform(0, 250, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(300, 50, 0);
        glVertex3f(550, 50, 0);
        glVertex3f(550, 80, 0);
        glVertex3f(300, 80, 0);
        glEnd();
        storePlatform(300, 550, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(600, 50, 0);
        glVertex3f(900, 50, 0);
        glVertex3f(900, 80, 0);
        glVertex3f(600, 80, 0);
        glEnd();
        storePlatform(600, 900, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(250, 180, 0);
        glVertex3f(400, 180, 0);
        glVertex3f(400, 210, 0);
        glVertex3f(250, 210, 0);
        glEnd();
        storePlatform(250, 400, 180, 30);

        glColor3f(0.2f, 0.7f, 0.3f);
        drawDDA(600, 280, 750, 280);
        storePlatform(600, 750, 280, 30);
    }
    else
    {
        glColor3f(0.3f, 0.5f, 0.9f);

        glBegin(GL_QUADS);
        glVertex3f(0, 50, 0);
        glVertex3f(180, 50, 0);
        glVertex3f(180, 80, 0);
        glVertex3f(0, 80, 0);
        glEnd();
        storePlatform(0, 180, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(250, 50, 0);
        glVertex3f(450, 50, 0);
        glVertex3f(450, 80, 0);
        glVertex3f(250, 80, 0);
        glEnd();
        storePlatform(250, 450, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(520, 50, 0);
        glVertex3f(900, 50, 0);
        glVertex3f(900, 80, 0);
        glVertex3f(520, 80, 0);
        glEnd();
        storePlatform(520, 900, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(350, 180, 0);
        glVertex3f(500, 180, 0);
        glVertex3f(500, 210, 0);
        glVertex3f(350, 210, 0);
        glEnd();
        storePlatform(350, 500, 180, 30);

        glBegin(GL_QUADS);
        glVertex3f(650, 280, 0);
        glVertex3f(800, 280, 0);
        glVertex3f(800, 310, 0);
        glVertex3f(650, 310, 0);
        glEnd();
        storePlatform(650, 800, 280, 30);
    }

    if(!crystal1)
        drawCrystal(210, 210);

    if(!crystal2)
        drawCrystal(450, 210);

    if(!crystal3)
        drawCrystal(650, 310);

    if(crystal1 && crystal2 && crystal3)
        drawPortal(820, 110);
}

// [OBJ-06] Timeline - Level 5
void level5()
{
    platformCount = 0;

    if(timeline == 0)
    {
        glColor3f(0.2f, 0.7f, 0.3f);

        glBegin(GL_QUADS);
        glVertex3f(0, 50, 0);
        glVertex3f(250, 50, 0);
        glVertex3f(250, 80, 0);
        glVertex3f(0, 80, 0);
        glEnd();
        storePlatform(0, 250, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(300, 50, 0);
        glVertex3f(550, 50, 0);
        glVertex3f(550, 80, 0);
        glVertex3f(300, 80, 0);
        glEnd();
        storePlatform(300, 550, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(600, 50, 0);
        glVertex3f(900, 50, 0);
        glVertex3f(900, 80, 0);
        glVertex3f(600, 80, 0);
        glEnd();
        storePlatform(600, 900, 50, 30);
    }
    else
    {
        glColor3f(0.3f, 0.5f, 0.9f);

        glBegin(GL_QUADS);
        glVertex3f(0, 50, 0);
        glVertex3f(250, 50, 0);
        glVertex3f(250, 80, 0);
        glVertex3f(0, 80, 0);
        glEnd();
        storePlatform(0, 250, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(350, 50, 0);
        glVertex3f(530, 50, 0);
        glVertex3f(530, 80, 0);
        glVertex3f(350, 80, 0);
        glEnd();
        storePlatform(350, 530, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(650, 50, 0);
        glVertex3f(900, 50, 0);
        glVertex3f(900, 80, 0);
        glVertex3f(650, 80, 0);
        glEnd();
        storePlatform(650, 900, 50, 30);

        glBegin(GL_QUADS);
        glVertex3f(300, 180, 0);
        glVertex3f(420, 180, 0);
        glVertex3f(420, 210, 0);
        glVertex3f(300, 210, 0);
        glEnd();
        storePlatform(300, 420, 180, 30);

        glBegin(GL_QUADS);
        glVertex3f(550, 280, 0);
        glVertex3f(670, 280, 0);
        glVertex3f(670, 310, 0);
        glVertex3f(550, 310, 0);
        glEnd();
        storePlatform(550, 670, 280, 30);
    }

    glEnable(GL_DEPTH_TEST);

    drawCube(227.5f, 107.5f, angle1, switch1);
    drawCube(577.5f, 347.5f, angle2, switch2);
    drawCube(717.5f, 107.5f, angle3, switch3);

    glDisable(GL_DEPTH_TEST);
    glLoadIdentity();

    if(switch1 && switch2 && switch3)
        drawPortal(820, 140);
}

void loadLevel()
{
    platformCount = 0;

    if(level == 1)
        level1();
    else if(level == 2)
        level2();
    else if(level == 3)
        level3();
    else if(level == 4)
        level4();
    else if(level == 5)
        level5();
}

// [ANIM-04] Reset Player
void resetPlayer()
{
    tx = 50;
    ty = 80;

    velocityX = 0.0f;
    velocityY = 0.0f;

    timeline = 0;
}

// [ANIM-01] Game Update and Animation
// [ANIM-05] Rotating Cube Animation is updated here
void update(int value)
{
    loadLevel();

    bool movingLeft = keyDown['a'] || keyDown['A'];
    bool movingRight = keyDown['d'] || keyDown['D'];

    if(movingLeft && !movingRight)
    {
        velocityX -= acceleration;
        if(velocityX < -moveSpeed)
            velocityX = -moveSpeed;
    }

    if(movingRight && !movingLeft)
    {
        velocityX += acceleration;
        if(velocityX > moveSpeed)
            velocityX = moveSpeed;
    }

    if(!movingLeft && !movingRight)
    {
        velocityX *= friction;
        if(fabs(velocityX) < 0.05f)
            velocityX = 0.0f;
    }

    tx += velocityX;

    if(tx < 0)
    {
        tx = 0;
        velocityX = 0;
    }

    if(tx > 870)
    {
        tx = 870;
        velocityX = 0;
    }

    velocityY += gravity;
    ty += velocityY;

    onGround = false;

    if(velocityY <= 0)
    {
        for(int i = 0; i < platformCount; i++)
        {
            Platform p = platforms[i];
            bool horizontal = tx + 25 > p.x1 && tx + 5 < p.x2;
            float playerBottom = ty;
            float previousBottom = ty - velocityY;

            if(horizontal && previousBottom >= p.y && playerBottom <= p.y)
            {
                ty = p.y;
                velocityY = 0;
                onGround = true;
                break;
            }
        }
    }

    if(ty < -80)
    {
        cout << "You fell! Restarting level " << level << endl;
        resetPlayer();
    }

    if(level == 1)
    {
        if(timeline == 0 && tx > 250 && tx < 650 && ty <= 100)
        {
            cout << "You fell into the gap in Level 1." << endl;
            resetPlayer();
        }

        if(tx > 780 && ty >= 40)
        {
            cout << "Level 1 Complete!" << endl;
            cout << "Starting Level 2..." << endl;
            level = 2;
            resetPlayer();
        }
    }

    if(level == 2)
    {
        if(timeline == 0 && tx > 780 && ty >= 40)
        {
            cout << "Level 2 cannot be completed in PAST." << endl;
            cout << "Switch to FUTURE using T." << endl;
        }

        if(timeline == 1 && tx > 300 && tx < 600 && ty <= 100)
        {
            cout << "You fell into the gap in Level 2." << endl;
            resetPlayer();
        }

        if(timeline == 1 && tx > 780 && ty >= 40)
        {
            cout << "Level 2 Complete!" << endl;
            cout << "Starting Level 3..." << endl;
            level = 3;
            resetPlayer();
        }
    }

    if(level == 3)
    {
        if(tx > 760 && ty > 350)
        {
            cout << "Level 3 Complete!" << endl;
            cout << "Starting Level 4..." << endl;
            level = 4;
            resetPlayer();
        }
    }

    if(level == 4)
    {
        if(!crystal1 && tx > 190 && tx < 260 && ty > 170)
        {
            crystal1 = true;
            cout << "Crystal 1 collected!" << endl;
        }

        if(!crystal2 && tx > 420 && tx < 500 && ty > 170)
        {
            crystal2 = true;
            cout << "Crystal 2 collected!" << endl;
        }

        if(!crystal3 && tx > 620 && tx < 710 && ty > 270)
        {
            crystal3 = true;
            cout << "Crystal 3 collected!" << endl;
        }

        if(crystal1 && crystal2 && crystal3 && tx > 780 && ty >= 40)
        {
            cout << "All crystals collected!" << endl;
            cout << "Level 4 Complete!" << endl;
            cout << "Starting Level 5..." << endl;
            level = 5;
            resetPlayer();
        }
    }

    if(level == 5)
    {
        if(!switch1 && tx + 30 > 207 && tx < 248 && ty + 40 > 87 && ty < 128)
        {
            switch1 = true;
            cout << "Cube 1 activated!" << endl;
        }

        if(!switch2 && tx + 30 > 557 && tx < 598 && ty + 40 > 327 && ty < 368)
        {
            switch2 = true;
            cout << "Cube 2 activated!" << endl;
        }

        if(!switch3 && tx + 30 > 697 && tx < 738 && ty + 40 > 87 && ty < 128)
        {
            switch3 = true;
            cout << "Cube 3 activated!" << endl;
        }

        if(switch1 && switch2 && switch3 && tx > 780)
        {
            gameComplete = true;
            cout << endl;
            cout << "YOU WIN!" << endl;
        }
    }

    angle1 += 1.0f;
    angle2 += 1.0f;
    angle3 += 1.0f;

    if(angle1 >= 360.0f) angle1 -= 360.0f;
    if(angle2 >= 360.0f) angle2 -= 360.0f;
    if(angle3 >= 360.0f) angle3 -= 360.0f;

    glutPostRedisplay();
    glutTimerFunc(16, update, 0);
}

// [ANIM-02] Keyboard Input, Jump and Timeline Shift
// [ANIM-06] Timeline Switching is handled here
void keyboardDown(unsigned char key, int x, int y)
{
    if(!keyDown[key])
    {
        keyDown[key] = true;

        switch(key)
        {
            case 'w':
            case 'W':
            case ' ':
                if(onGround)
                {
                    velocityY = jumpPower;
                    onGround = false;
                }
                break;

            case 't':
            case 'T':
                timeline = 1 - timeline;
                velocityY = 0;
                onGround = false;

                if(timeline == 0)
                    cout << "Time Shift: PAST" << endl;
                else
                    cout << "Time Shift: FUTURE" << endl;
                break;

            case 'n':
            case 'N':
                if(level < 5)
                {
                    level++;
                    cout << "Skipped to Level " << level << endl;
                    resetPlayer();
                }
                break;
        }
    }
    glutPostRedisplay();
}

// [ANIM-03] Keyboard Release
void keyboardUp(unsigned char key, int x, int y)
{
    keyDown[key] = false;
}

void display()
{
    if(timeline == 0)
        glClearColor(0.55f, 0.75f, 0.95f, 1.0f);
    else
        glClearColor(0.08f, 0.08f, 0.20f, 1.0f);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_DEPTH_TEST);

    if(level == 1)
        level1();
    else if(level == 2)
        level2();
    else if(level == 3)
        level3();
    else if(level == 4)
        level4();
    else if(level == 5)
        level5();

    glDisable(GL_DEPTH_TEST);
    glLoadIdentity();
    drawPlayer();

    glutSwapBuffers();
}

void init()
{
    glClearColor(0.55f, 0.75f, 0.95f, 1.0f);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, 900.0, 0.0, 600.0, -100.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

int main(int argc, char** argv)
{
    cout << "         TIMESHIFT" << endl;

    cout << "A / D     : Move" << endl;
    cout << "W / SPACE : Jump" << endl;
    cout << "T         : Time Shift" << endl;
    cout << "N         : Next Level" << endl;
    cout << endl;
    cout << "Starting Level 1..." << endl;
    cout << endl;

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(900, 600);
    glutCreateWindow("TIMESHIFT");

    init();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboardDown);
    glutKeyboardUpFunc(keyboardUp);
    glutTimerFunc(16, update, 0);

    glutMainLoop();
    return 0;
}
