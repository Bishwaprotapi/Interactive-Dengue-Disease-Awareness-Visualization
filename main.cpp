/*
 * Interactive Dengue Disease Awareness Visualization
 * An educational tool designed for young students and illiterate populations
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>
#include <map>
#include <algorithm>

// Scene and animation management
static int currentScene = 0;
static const int TOTAL_SCENES = 9;
static float waterLevel = 0.0f;
static bool waterRising = true;
static int selectedItem = -1;
static bool isAnimating = true;
static bool isDayMode = true;
static bool showAmbulance = true;
static bool showEmergency = true;
static bool showInfo = true;
static bool showOptions = false;
static bool showUpdates = false;
static int killedMosquitoes = 0;

// Add new animation variables
static float rippleTime = 0.0f;
static float mosquitoLifeCycle = 0.0f;
static float pulseEffect = 0.0f;
static float rainEffect = 0.0f;
static bool isRaining = false;
static float mosquitoSpawnTimer = 0.0f;
static float mosquitoSpawnInterval = 2.0f;
static float mosquitoSpeed = 0.02f;
static float mosquitoWingSpeed = 0.1f;
static float mosquitoDeathTimer = 0.0f;
static bool mosquitoDeathAnimation = false;

// Add mosquito kill effect variables
static float killEffectTime = 0.0f;
static int mosquitoToKill = -1;
static bool isKillingMosquito = false;

// Add new animation variables for people
struct Person {
    float x, y;
    float scale;
    bool isMoving;
    float moveDirection;
    float animationTime;
    int type; // 0: fighter, 1: cleaner, 2: inspector, 3: patient
};

std::vector<Person> people;
const int MAX_MOSQUITOES = 20;
const int MIN_MOSQUITOES = 0;
static float mosquitoSpawnRate = 0.5f; // Mosquitoes per second

// Add statistics data
struct Statistics {
    int year;
    int cases;
    int deaths;
    int prevention;
};

std::vector<Statistics> yearlyStats = {
    {2018, 1200, 15, 800},
    {2019, 1500, 20, 1000},
    {2020, 1800, 25, 1200},
    {2021, 1400, 18, 1500},
    {2022, 1000, 12, 1800},
    {2023, 800, 8, 2000}
};

// Colors
struct Color {
    float r, g, b;
};

const Color DAY_SKY = {0.529f, 0.808f, 0.922f};
const Color NIGHT_SKY = {0.1f, 0.1f, 0.2f};
const Color GRASS = {0.196f, 0.804f, 0.196f};
const Color HOUSE = {0.8f, 0.4f, 0.2f};
const Color WATER = {0.0f, 0.5f, 1.0f};
const Color AMBULANCE = {1.0f, 0.0f, 0.0f};

// Scene names
const std::map<int, std::string> SCENE_NAMES = {
    {0, "Mosquito Journey Scene"},
    {1, "Clean Environment Scene"},
    {2, "Dengue Fighter Scene"},
    {3, "Stagnant Water Scene"},
    {4, "Home Inspection Scene"},
    {5, "Symptoms Scene"},
    {6, "Prevention Methods Scene"},
    {7, "Treatment Options Scene"},
    {8, "Statistics Scene"}
};

// Interactive elements
struct InteractiveElement {
    float x, y;
    float width, height;
    int scene;
    bool isHovered;
    bool isClicked;
};

std::vector<InteractiveElement> interactiveElements;

// Mosquito struct for multiple mosquitoes
struct Mosquito {
    float x, y;
    float speedX, speedY;
    float wingAngle;
    bool alive;
};

std::vector<Mosquito> mosquitoes;
const int MOSQUITO_COUNT = 10;

// Information display system
struct InfoMessage {
    std::string text;
    float displayTime;
    float fadeTime;
    bool active;
};

std::vector<InfoMessage> infoMessages;
float messageTimer = 0.0f;
const float MESSAGE_DURATION = 5.0f; // seconds per message

// Scene-specific information
const std::map<int, std::vector<std::string>> SCENE_INFO = {
    {0, {
        "Mosquitoes go through four life stages: egg, larva, pupa, and adult",
        "Female Aedes aegypti mosquitoes lay eggs in stagnant water",
        "After hatching, they become larvae, then pupae, and finally adults",
        "This scene shows how unchecked water sources contribute to breeding"
    }},
    {1, {
        "A clean environment is the first defense against dengue",
        "Remove stagnant water from plant pots, buckets, old tires, and drains",
        "Proper garbage disposal and cleaning roof gutters helps",
        "Using mosquito nets reduces breeding grounds for mosquitoes"
    }},
    {2, {
        "Dengue Fighters are everyday heroes in our community",
        "They include community workers, health officials, and citizens",
        "They raise awareness and inspect areas for breeding grounds",
        "You can be a Dengue Fighter by inspecting your surroundings"
    }},
    {3, {
        "Stagnant water is a mosquito's favorite breeding place",
        "Common spots: flowerpots, unused containers, air coolers",
        "Empty and clean these spots weekly",
        "Even a bottle cap full of water is enough for breeding"
    }},
    {4, {
        "Regular home inspections help detect breeding areas early",
        "Check bathrooms, balconies, and rooftop tanks",
        "Don't forget refrigerator trays and water containers",
        "Do a weekly '10-Minute Check' to eliminate standing water"
    }},
    {5, {
        "Common symptoms: High fever (up to 104�F)",
        "Severe headache, pain behind eyes, muscle and joint pain",
        "Nausea, vomiting, and skin rash may occur",
        "Bleeding (gums, nose) in severe cases - Seek help immediately"
    }},
    {6, {
        "Prevent dengue by using repellents and mosquito nets",
        "Wear long-sleeved clothes and install window screens",
        "Cover water containers and apply larvicides",
        "Raise community awareness about prevention"
    }},
    {7, {
        "No specific cure, but early diagnosis saves lives",
        "Treatment includes rest and plenty of fluids",
        "Use paracetamol for fever (avoid aspirin/NSAIDs)",
        "Hospital care needed for severe dengue - Don't self-medicate"
    }},
    {8, {
        "Dengue cases are rising globally due to urbanization",
        "Climate change increases mosquito breeding areas",
        "Urban areas are at higher risk of outbreaks",
        "Stay informed through health department updates"
    }}
};

// Texture IDs
GLuint textureIDs[20]; // Array to store texture IDs

// Load texture function using standard OpenGL
GLuint loadTexture(const unsigned char* data, int width, int height) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture data
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    return textureID;
}

// Initialize textures with default colors
void initTextures() {
    // Create simple colored textures
    unsigned char mosquitoData[4] = {200, 200, 200, 255}; // Gray
    unsigned char waterData[4] = {0, 128, 255, 200};      // Blue
    unsigned char houseData[4] = {204, 102, 51, 255};     // Brown
    unsigned char personData[4] = {255, 204, 153, 255};   // Skin tone
    unsigned char ambulanceData[4] = {255, 0, 0, 255};    // Red
    unsigned char grassData[4] = {50, 205, 50, 255};      // Green
    unsigned char skyDayData[4] = {135, 206, 235, 255};   // Light blue
    unsigned char skyNightData[4] = {25, 25, 51, 255};    // Dark blue
    unsigned char rainData[4] = {200, 200, 255, 128};     // Light blue with alpha
    unsigned char rippleData[4] = {0, 128, 255, 128};     // Blue with alpha

    // Load textures
    textureIDs[0] = loadTexture(mosquitoData, 1, 1);
    textureIDs[1] = loadTexture(waterData, 1, 1);
    textureIDs[2] = loadTexture(houseData, 1, 1);
    textureIDs[3] = loadTexture(personData, 1, 1);
    textureIDs[4] = loadTexture(ambulanceData, 1, 1);
    textureIDs[5] = loadTexture(grassData, 1, 1);
    textureIDs[6] = loadTexture(skyDayData, 1, 1);
    textureIDs[7] = loadTexture(skyNightData, 1, 1);
    textureIDs[8] = loadTexture(rainData, 1, 1);
    textureIDs[9] = loadTexture(rippleData, 1, 1);
}

// Function declarations
void addInteractiveElement(float x, float y, float width, float height, int scene);
void initInteractiveElements();
void initMosquitoes();
void initInfoMessages();
void initPeople();
void startAutomaticDisplay();
void drawOptionsBox();
void drawWaterRipple(float x, float y, float scale, float time);
void drawMosquitoLifeCycle(float x, float y, float scale, float cycle);
void drawRain();
void drawPulseEffect(float x, float y, float scale);
void drawMosquitoKillEffect(float x, float y, float scale, float time);
void drawSceneNameBox();

// Function definitions
void addInteractiveElement(float x, float y, float width, float height, int scene) {
    InteractiveElement element = {x, y, width, height, scene, false, false};
    interactiveElements.push_back(element);
}

void initInteractiveElements() {
    // Add interactive elements for each scene
    addInteractiveElement(-3.0f, 1.0f, 1.0f, 1.0f, 1); // Water container
    addInteractiveElement(0.0f, 0.0f, 1.0f, 1.0f, 1); // Person with net
    addInteractiveElement(3.0f, 1.0f, 1.0f, 1.0f, 1); // Mosquito repellent
}

void initMosquitoes() {
    mosquitoes.clear();
    if (currentScene == 0) {
        mosquitoes.push_back({-2.0f, 2.0f, 0.02f, -0.01f, 0.0f, true});
    } else {
        for(int i = 0; i < MOSQUITO_COUNT; ++i) {
            float x = -4.0f + static_cast<float>(rand()) / RAND_MAX * 8.0f;
            float y = -2.0f + static_cast<float>(rand()) / RAND_MAX * 6.0f;
            float speedX = 0.01f + static_cast<float>(rand()) / RAND_MAX * 0.03f;
            float speedY = 0.01f + static_cast<float>(rand()) / RAND_MAX * 0.03f;
            if(rand() % 2) speedX = -speedX;
            if(rand() % 2) speedY = -speedY;
            mosquitoes.push_back({x, y, speedX, speedY, 0.0f, true});
        }
    }
}

void initInfoMessages() {
    infoMessages.clear();
    auto it = SCENE_INFO.find(currentScene);
    if(it != SCENE_INFO.end()) {
        const auto& messages = it->second;
        for(const auto& msg : messages) {
            infoMessages.push_back({msg, 0.0f, 0.0f, false});
        }
        if(!infoMessages.empty()) {
            infoMessages[0].active = true;
        }
    }
}

void startAutomaticDisplay() {
    // Initialize all displays
    initInfoMessages();
    initMosquitoes();
    initInteractiveElements();
    initPeople();

    // Start with first scene
    currentScene = 0;

    // Enable all displays
    isAnimating = true;
    showInfo = true;
    showAmbulance = true;
    showEmergency = true;
}

// Check if point is inside an element
bool isPointInside(float x, float y, const InteractiveElement& element) {
    return (x >= element.x - element.width/2 && x <= element.x + element.width/2 &&
            y >= element.y - element.height/2 && y <= element.y + element.height/2);
}

// Draw a mosquito with animation
void drawMosquito(float x, float y, float scale, float wingAngle) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);

    // Body
    glColor3f(0.2f, 0.2f, 0.2f);
    glBegin(GL_LINES);
    glVertex2f(0, 0);
    glVertex2f(0, 0.5f);
    glEnd();

    // Wings with enhanced animation
    glColor4f(0.3f, 0.3f, 0.3f, 0.8f);
    glBegin(GL_TRIANGLES);
    // Left wing with smoother animation
    float wingAngle1 = sin(wingAngle * mosquitoWingSpeed) * 0.5f;
    glVertex2f(0, 0.3f);
    glVertex2f(-0.2f * cos(wingAngle1), 0.4f);
    glVertex2f(0, 0.5f);

    // Right wing with smoother animation
    glVertex2f(0, 0.3f);
    glVertex2f(0.2f * cos(wingAngle1), 0.4f);
    glVertex2f(0, 0.5f);
    glEnd();

    // Add a small dot for the head
    glColor3f(0.1f, 0.1f, 0.1f);
    glPointSize(3.0f);
    glBegin(GL_POINTS);
    glVertex2f(0, 0.5f);
    glEnd();

    glPopMatrix();
}

// Draw a water container with animated water level
void drawWaterContainer(float x, float y, float scale, float waterLevel) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);

    // Container
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);

    // Draw container with texture
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-0.3f, -0.2f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(0.3f, -0.2f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(0.3f, 0.2f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-0.3f, 0.2f);
    glEnd();

    // Water with animated texture
    glBindTexture(GL_TEXTURE_2D, textureIDs[1]);
    glBegin(GL_QUADS);
    float texY = waterLevel;
    glTexCoord2f(0.0f, texY); glVertex2f(-0.25f, -0.15f);
    glTexCoord2f(1.0f, texY); glVertex2f(0.25f, -0.15f);
    glTexCoord2f(1.0f, texY + 0.3f); glVertex2f(0.25f, -0.15f + waterLevel * 0.3f);
    glTexCoord2f(0.0f, texY + 0.3f); glVertex2f(-0.25f, -0.15f + waterLevel * 0.3f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

// Enhanced drawPerson function with different types
void drawPerson(float x, float y, float scale, int type, float animationTime) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);

    // Body color based on type
    switch(type) {
        case 0: // Fighter
            glColor3f(0.0f, 0.5f, 1.0f); // Blue uniform
            break;
        case 1: // Cleaner
            glColor3f(0.0f, 0.8f, 0.0f); // Green uniform
            break;
        case 2: // Inspector
            glColor3f(1.0f, 0.5f, 0.0f); // Orange uniform
            break;
        case 3: // Patient
            glColor3f(1.0f, 0.8f, 0.8f); // Light red
            break;
    }

    // Animated body movement
    float bodyOffset = sin(animationTime * 2.0f) * 0.05f;

    // Body
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, 0.3f + bodyOffset);
    glVertex2f(0.1f, 0.3f + bodyOffset);
    glVertex2f(0.1f, -0.1f + bodyOffset);
    glVertex2f(-0.1f, -0.1f + bodyOffset);
    glEnd();

    // Head
    glColor3f(1.0f, 0.8f, 0.6f);
    glBegin(GL_TRIANGLE_FAN);
    for(int i = 0; i < 360; i += 10) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(0.1f * cos(angle), 0.1f * sin(angle) + 0.4f + bodyOffset);
    }
    glEnd();

    // Arms with animation
    float armAngle = sin(animationTime * 4.0f) * 0.3f;
    glColor3f(0.0f, 0.5f, 1.0f);
    glBegin(GL_LINES);
    // Left arm
    glVertex2f(-0.1f, 0.2f + bodyOffset);
    glVertex2f(-0.2f * cos(armAngle), 0.1f + bodyOffset);
    // Right arm
    glVertex2f(0.1f, 0.2f + bodyOffset);
    glVertex2f(0.2f * cos(armAngle), 0.1f + bodyOffset);
    glEnd();

    // Legs with animation
    float legAngle = sin(animationTime * 4.0f + 3.14159f) * 0.2f;
    glBegin(GL_LINES);
    // Left leg
    glVertex2f(-0.05f, -0.1f + bodyOffset);
    glVertex2f(-0.1f * cos(legAngle), -0.3f + bodyOffset);
    // Right leg
    glVertex2f(0.05f, -0.1f + bodyOffset);
    glVertex2f(0.1f * cos(legAngle), -0.3f + bodyOffset);
    glEnd();

    // Add equipment based on type
    switch(type) {
        case 0: // Fighter with spray
            glColor3f(0.5f, 0.5f, 0.5f);
            glBegin(GL_LINES);
            glVertex2f(0.2f * cos(armAngle), 0.1f + bodyOffset);
            glVertex2f(0.4f * cos(armAngle), 0.0f + bodyOffset);
            glEnd();
            break;
        case 1: // Cleaner with broom
            glColor3f(0.6f, 0.3f, 0.1f);
            glBegin(GL_LINES);
            glVertex2f(-0.2f * cos(armAngle), 0.1f + bodyOffset);
            glVertex2f(-0.4f * cos(armAngle), -0.1f + bodyOffset);
            glEnd();
            break;
        case 2: // Inspector with clipboard
            glColor3f(1.0f, 1.0f, 1.0f);
            glBegin(GL_QUADS);
            glVertex2f(0.2f * cos(armAngle), 0.1f + bodyOffset);
            glVertex2f(0.3f * cos(armAngle), 0.1f + bodyOffset);
            glVertex2f(0.3f * cos(armAngle), 0.0f + bodyOffset);
            glVertex2f(0.2f * cos(armAngle), 0.0f + bodyOffset);
            glEnd();
            break;
        case 3: // Patient with thermometer
            glColor3f(1.0f, 0.0f, 0.0f);
            glBegin(GL_LINES);
            glVertex2f(0.2f * cos(armAngle), 0.1f + bodyOffset);
            glVertex2f(0.3f * cos(armAngle), 0.2f + bodyOffset);
            glEnd();
            break;
    }

    glPopMatrix();
}

// Draw background
void drawBackground() {
    glEnable(GL_TEXTURE_2D);

    // Sky
    if(isDayMode) {
        glBindTexture(GL_TEXTURE_2D, textureIDs[6]);
    } else {
        glBindTexture(GL_TEXTURE_2D, textureIDs[7]);
    }

    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-5.0f, -5.0f);
    glTexCoord2f(1.0f, 0.0f); glVertex2f(5.0f, -5.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex2f(5.0f, 5.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-5.0f, 5.0f);
    glEnd();

    // Ground with grass texture
    glBindTexture(GL_TEXTURE_2D, textureIDs[5]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex2f(-5.0f, -5.0f);
    glTexCoord2f(2.0f, 0.0f); glVertex2f(5.0f, -5.0f);
    glTexCoord2f(2.0f, 1.0f); glVertex2f(5.0f, -3.0f);
    glTexCoord2f(0.0f, 1.0f); glVertex2f(-5.0f, -3.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Draw a house
void drawHouse(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);

    // House body
    glColor3f(HOUSE.r, HOUSE.g, HOUSE.b);
    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);
    glVertex2f(0.5f, -0.5f);
    glVertex2f(0.5f, 0.5f);
    glVertex2f(-0.5f, 0.5f);
    glEnd();

    // Roof
    glColor3f(0.5f, 0.2f, 0.1f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.6f, 0.5f);
    glVertex2f(0.0f, 0.8f);
    glVertex2f(0.6f, 0.5f);
    glEnd();

    // Door
    glColor3f(0.4f, 0.2f, 0.1f);
    glBegin(GL_QUADS);
    glVertex2f(-0.1f, -0.5f);
    glVertex2f(0.1f, -0.5f);
    glVertex2f(0.1f, 0.0f);
    glVertex2f(-0.1f, 0.0f);
    glEnd();

    glPopMatrix();
}

// Draw an ambulance
void drawAmbulance(float x, float y, float scale) {
    if(!showAmbulance) return;

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);

    // Ambulance body
    glColor3f(AMBULANCE.r, AMBULANCE.g, AMBULANCE.b);
    glBegin(GL_QUADS);
    glVertex2f(-0.8f, -0.4f);
    glVertex2f(0.8f, -0.4f);
    glVertex2f(0.8f, 0.4f);
    glVertex2f(-0.8f, 0.4f);
    glEnd();

    // Cross symbol
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glVertex2f(-0.2f, -0.2f);
    glVertex2f(0.2f, -0.2f);
    glVertex2f(0.2f, 0.2f);
    glVertex2f(-0.2f, 0.2f);
    glEnd();

    glPopMatrix();
}

// Draw emergency contacts
void drawEmergencyContacts() {
    if(!showEmergency) return;

    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-4.0f, 3.0f);
    glVertex2f(4.0f, 3.0f);
    glVertex2f(4.0f, 4.0f);
    glVertex2f(-4.0f, 4.0f);
    glEnd();
}

// Enhanced drawStatistics function
void drawStatistics() {
    // Draw background
    glColor4f(0.2f, 0.2f, 0.2f, 0.8f);
    glBegin(GL_QUADS);
    glVertex2f(-4.5f, -4.5f);
    glVertex2f(4.5f, -4.5f);
    glVertex2f(4.5f, 4.5f);
    glVertex2f(-4.5f, 4.5f);
    glEnd();

    // Draw title
    glColor3f(1.0f, 1.0f, 1.0f);
    glRasterPos2f(-1.0f, 4.0f);
    std::string title = "Dengue Statistics (2018-2023)";
    for(const char& c : title) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }

    // Draw bars for each year
    float barWidth = 0.8f;
    float maxHeight = 3.0f;
    float startX = -3.5f;
    float spacing = 1.2f;

    for(size_t i = 0; i < yearlyStats.size(); i++) {
        float x = startX + i * spacing;

        // Cases bar (red)
        glColor3f(1.0f, 0.0f, 0.0f);
        float casesHeight = (yearlyStats[i].cases / 2000.0f) * maxHeight;
        glBegin(GL_QUADS);
        glVertex2f(x, -3.0f);
        glVertex2f(x + barWidth, -3.0f);
        glVertex2f(x + barWidth, -3.0f + casesHeight);
        glVertex2f(x, -3.0f + casesHeight);
        glEnd();

        // Deaths bar (dark red)
        glColor3f(0.7f, 0.0f, 0.0f);
        float deathsHeight = (yearlyStats[i].deaths / 30.0f) * maxHeight;
        glBegin(GL_QUADS);
        glVertex2f(x + barWidth * 0.25f, -3.0f);
        glVertex2f(x + barWidth * 0.75f, -3.0f);
        glVertex2f(x + barWidth * 0.75f, -3.0f + deathsHeight);
        glVertex2f(x + barWidth * 0.25f, -3.0f + deathsHeight);
        glEnd();

        // Prevention bar (green)
        glColor3f(0.0f, 1.0f, 0.0f);
        float preventionHeight = (yearlyStats[i].prevention / 2000.0f) * maxHeight;
        glBegin(GL_QUADS);
        glVertex2f(x + barWidth * 0.5f, -3.0f);
        glVertex2f(x + barWidth, -3.0f);
        glVertex2f(x + barWidth, -3.0f + preventionHeight);
        glVertex2f(x + barWidth * 0.5f, -3.0f + preventionHeight);
        glEnd();

        // Year label
        glColor3f(1.0f, 1.0f, 1.0f);
        glRasterPos2f(x + barWidth * 0.5f - 0.1f, -3.2f);
        std::string year = std::to_string(yearlyStats[i].year);
        for(const char& c : year) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, c);
        }
    }
}

// Modify updateMosquitoes function to control population
void updateMosquitoes() {
    mosquitoSpawnTimer += 0.016f;

    // Spawn new mosquitoes if below maximum
    if(mosquitoSpawnTimer >= mosquitoSpawnInterval && mosquitoes.size() < MAX_MOSQUITOES) {
        mosquitoSpawnTimer = 0.0f;
        float x = -4.5f + static_cast<float>(rand()) / RAND_MAX * 9.0f;
        float y = -2.5f + static_cast<float>(rand()) / RAND_MAX * 7.0f;
        float speedX = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * mosquitoSpeed;
        float speedY = (static_cast<float>(rand()) / RAND_MAX - 0.5f) * mosquitoSpeed;
        mosquitoes.push_back({x, y, speedX, speedY, 0.0f, true});
    }

    // Update existing mosquitoes
    for(auto& m : mosquitoes) {
        if(!m.alive) continue;

        // Update position with smooth movement
        m.x += m.speedX;
        m.y += m.speedY;
        m.wingAngle += 0.1f;

        // Bounce off boundaries with smooth deceleration
        if(m.x < -4.5f || m.x > 4.5f) {
            m.speedX = -m.speedX * 0.95f;
            m.x = m.x < -4.5f ? -4.5f : 4.5f;
        }
        if(m.y < -2.5f || m.y > 4.5f) {
            m.speedY = -m.speedY * 0.95f;
            m.y = m.y < -2.5f ? -2.5f : 4.5f;
        }

        // Add slight random movement
        m.speedX += (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.001f;
        m.speedY += (static_cast<float>(rand()) / RAND_MAX - 0.5f) * 0.001f;

        // Limit maximum speed
        float maxSpeed = mosquitoSpeed * 1.5f;
        float currentSpeed = sqrt(m.speedX * m.speedX + m.speedY * m.speedY);
        if(currentSpeed > maxSpeed) {
            m.speedX = (m.speedX / currentSpeed) * maxSpeed;
            m.speedY = (m.speedY / currentSpeed) * maxSpeed;
        }
    }

    // Remove dead mosquitoes after animation
    if(mosquitoDeathAnimation) {
        mosquitoDeathTimer += 0.016f;
        if(mosquitoDeathTimer >= 1.0f) {
            mosquitoDeathAnimation = false;
            mosquitoDeathTimer = 0.0f;
            mosquitoes.erase(
                std::remove_if(mosquitoes.begin(), mosquitoes.end(),
                    [](const Mosquito& m) { return !m.alive; }),
                mosquitoes.end()
            );
        }
    }
}

void drawAllMosquitoes() {
    for(const auto& m : mosquitoes) {
        if(m.alive)
            drawMosquito(m.x, m.y, 0.5f, m.wingAngle);
    }
}

void updateInfoMessages(float deltaTime) {
    messageTimer += deltaTime;
    if(messageTimer >= MESSAGE_DURATION) {
        messageTimer = 0.0f;
        // Find current active message
        for(size_t i = 0; i < infoMessages.size(); ++i) {
            if(infoMessages[i].active) {
                infoMessages[i].active = false;
                // Activate next message
                if(i + 1 < infoMessages.size()) {
                    infoMessages[i + 1].active = true;
                } else {
                    infoMessages[0].active = true;
                }
                break;
            }
        }
    }
}

void drawInfoMessages() {
    if(!showInfo) return;

    for(const auto& msg : infoMessages) {
        if(msg.active) {
            // Draw message background at bottom
            glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
            glBegin(GL_QUADS);
            glVertex2f(-4.5f, -4.5f);
            glVertex2f(4.5f, -4.5f);
            glVertex2f(4.5f, -3.5f);
            glVertex2f(-4.5f, -3.5f);
            glEnd();

            // Draw message text at bottom
            glColor3f(1.0f, 1.0f, 1.0f);
            glRasterPos2f(-4.0f, -4.0f);
            for(const char& c : msg.text) {
                glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
            }
        }
    }
}

// Automatic scene transition
float sceneTimer = 0.0f;
const float SCENE_DURATION = 15.0f; // seconds per scene

void updateSceneTransition(float deltaTime) {
    sceneTimer += deltaTime;
    if(sceneTimer >= SCENE_DURATION) {
        sceneTimer = 0.0f;
        currentScene = (currentScene + 1) % TOTAL_SCENES;
        initInfoMessages();
    }
}

// Add function to draw water ripple effect
void drawWaterRipple(float x, float y, float scale, float time) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);

    // Draw multiple ripple circles
    for(int i = 0; i < 3; i++) {
        float radius = 0.1f + (time + i * 0.5f) * 0.2f;
        float alpha = 1.0f - (time + i * 0.5f) * 0.5f;

        glColor4f(0.0f, 0.5f, 1.0f, alpha);
        glBegin(GL_LINE_LOOP);
        for(int j = 0; j < 360; j += 10) {
            float angle = j * 3.14159f / 180.0f;
            glVertex2f(radius * cos(angle), radius * sin(angle));
        }
        glEnd();
    }
    glPopMatrix();
}

// Add function to draw mosquito life cycle
void drawMosquitoLifeCycle(float x, float y, float scale, float cycle) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);

    // Draw different stages based on cycle
    if(cycle < 0.25f) { // Egg stage
        glColor3f(0.8f, 0.8f, 0.8f);
        glBegin(GL_POINTS);
        for(int i = 0; i < 5; i++) {
            glVertex2f(i * 0.1f - 0.2f, 0.0f);
        }
        glEnd();
    }
    else if(cycle < 0.5f) { // Larva stage
        glColor3f(0.6f, 0.6f, 0.6f);
        glBegin(GL_LINES);
        glVertex2f(-0.2f, 0.0f);
        glVertex2f(0.2f, 0.0f);
        glEnd();
    }
    else if(cycle < 0.75f) { // Pupa stage
        glColor3f(0.4f, 0.4f, 0.4f);
        glBegin(GL_TRIANGLES);
        glVertex2f(-0.1f, -0.1f);
        glVertex2f(0.1f, -0.1f);
        glVertex2f(0.0f, 0.1f);
        glEnd();
    }
    else { // Adult stage
        drawMosquito(0.0f, 0.0f, 0.5f, cycle * 10.0f);
    }
    glPopMatrix();
}

// Add function to draw rain effect
void drawRain() {
    if(!isRaining) return;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureIDs[8]);

    glBegin(GL_QUADS);
    for(int i = 0; i < 50; i++) {
        float x = -4.5f + static_cast<float>(rand()) / RAND_MAX * 9.0f;
        float y = 4.5f - (rainEffect + static_cast<float>(rand()) / RAND_MAX * 2.0f);

        glTexCoord2f(0.0f, 0.0f); glVertex2f(x, y);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(x + 0.1f, y);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(x + 0.1f, y - 0.2f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(x, y - 0.2f);
    }
    glEnd();

    glDisable(GL_TEXTURE_2D);
}

// Add function to draw pulse effect
void drawPulseEffect(float x, float y, float scale) {
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);

    float radius = 0.2f + pulseEffect * 0.1f;
    float alpha = 1.0f - pulseEffect;

    glColor4f(1.0f, 0.0f, 0.0f, alpha);
    glBegin(GL_LINE_LOOP);
    for(int i = 0; i < 360; i += 10) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }
    glEnd();
    glPopMatrix();
}

// Add function to draw mosquito kill effect
void drawMosquitoKillEffect(float x, float y, float scale, float time) {
    if(time > 1.0f) return;

    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(scale, scale, scale);

    // Draw expanding circle with gradient
    float radius = time * 0.5f;
    glBegin(GL_TRIANGLE_FAN);
    glColor4f(1.0f, 0.0f, 0.0f, 1.0f - time);
    glVertex2f(0.0f, 0.0f);
    glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
    for(int i = 0; i <= 360; i += 10) {
        float angle = i * 3.14159f / 180.0f;
        glVertex2f(radius * cos(angle), radius * sin(angle));
    }
    glEnd();

    // Draw X mark with fade
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f - time);
    glLineWidth(2.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.2f, -0.2f);
    glVertex2f(0.2f, 0.2f);
    glVertex2f(-0.2f, 0.2f);
    glVertex2f(0.2f, -0.2f);
    glEnd();

    glPopMatrix();
}

// Modify updateAnimations function to include new animations
void updateAnimations(float deltaTime) {
    if(!isAnimating) return;

    // Update existing animations
    updateMosquitoes();
    updateInfoMessages(deltaTime);
    updateSceneTransition(deltaTime);

    // Update water level
    if(waterRising) {
        waterLevel += 0.01f * deltaTime;
        if(waterLevel > 1.0f) waterRising = false;
    } else {
        waterLevel -= 0.01f * deltaTime;
        if(waterLevel < 0.0f) waterRising = true;
    }

    // Update kill effect
    if(isKillingMosquito) {
        killEffectTime += deltaTime * 2.0f;
        if(killEffectTime > 1.0f) {
            isKillingMosquito = false;
            killEffectTime = 0.0f;
            if(mosquitoToKill >= 0 && static_cast<size_t>(mosquitoToKill) < mosquitoes.size()) {
                mosquitoes[mosquitoToKill].alive = false;
            }
            mosquitoToKill = -1;
        }
    }

    // Update new animations
    rippleTime += deltaTime;
    if(rippleTime > 2.0f) rippleTime = 0.0f;

    mosquitoLifeCycle += 0.1f * deltaTime;
    if(mosquitoLifeCycle > 1.0f) mosquitoLifeCycle = 0.0f;

    pulseEffect += 0.5f * deltaTime;
    if(pulseEffect > 1.0f) pulseEffect = 0.0f;

    rainEffect += 0.2f * deltaTime;
    if(rainEffect > 2.0f) rainEffect = 0.0f;

    // Randomly toggle rain
    if(rand() % 1000 < 1) isRaining = !isRaining;
}

// Add function to draw options box
void drawOptionsBox() {
    if(!showOptions) return;

    // Draw options background at top
    glColor4f(0.0f, 0.0f, 0.0f, 0.7f);
    glBegin(GL_QUADS);
    glVertex2f(-4.5f, 3.5f);
    glVertex2f(4.5f, 3.5f);
    glVertex2f(4.5f, 4.5f);
    glVertex2f(-4.5f, 4.5f);
    glEnd();

    // Draw options text, each on its own line
    glColor3f(1.0f, 1.0f, 1.0f);
    float y = 4.3f;
    float lineSpacing = 0.18f;
    const char* options[] = {
        "1: Mosquito Journey Scene",
        "2: Clean Environment Scene",
        "3: Dengue Fighter Scene",
        "4: Stagnant Water Scene",
        "5: Home Inspection Scene",
        "6: Symptoms Scene",
        "7: Prevention Methods Scene",
        "8: Treatment Options Scene",
        "9: Statistics Scene",
        "M/m: Toggle animation",
        "N/n: Toggle day/night mode",
        "D/d: Kill mosquitoes",
        "A/a: Show ambulance",
        "E/e: Emergency contacts",
        "I/i: Show information",
        "O/o: Show options",
        "U/u: Show updates"
    };
    for(int i = 0; i < 17; ++i) {
        glRasterPos2f(-4.3f, y - i * lineSpacing);
        const char* str = options[i];
        while(*str) {
            glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *str);
            ++str;
        }
    }
}

// Draw scene name at the top in the red box
void drawSceneNameBox() {
    // Draw red background at top
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(-4.5f, 3.5f);
    glVertex2f(4.5f, 3.5f);
    glVertex2f(4.5f, 4.5f);
    glVertex2f(-4.5f, 4.5f);
    glEnd();

    // Get current scene name
    std::string sceneName = SCENE_NAMES.at(currentScene);

    // Draw scene name centered
    glColor3f(1.0f, 1.0f, 1.0f);
    float nameLength = sceneName.length();
    float x = -0.15f * nameLength / 2.0f; // Centering adjustment
    glRasterPos2f(x, 4.0f);
    for (const char& c : sceneName) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

// Add function to initialize people
void initPeople() {
    people.clear();

    // Add fighters
    for(int i = 0; i < 3; i++) {
        float x = -3.0f + i * 3.0f;
        people.push_back({x, 0.0f, 1.0f, true, 1.0f, 0.0f, 0});
    }

    // Add cleaners
    for(int i = 0; i < 2; i++) {
        float x = -2.0f + i * 4.0f;
        people.push_back({x, -1.0f, 1.0f, true, -1.0f, 0.0f, 1});
    }

    // Add inspectors
    for(int i = 0; i < 2; i++) {
        float x = -3.0f + i * 6.0f;
        people.push_back({x, 0.0f, 1.0f, true, 1.0f, 0.0f, 2});
    }

    // Add patients
    for(int i = 0; i < 3; i++) {
        float x = -2.0f + i * 2.0f;
        people.push_back({x, 0.0f, 1.0f, false, 0.0f, 0.0f, 3});
    }
}

// Modify display function to include new scene elements
static void display(void)
{
    static float lastTime = 0.0f;
    float currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
    float deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Set up orthographic projection for 2D rendering
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-5.0, 5.0, -5.0, 5.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);

    // Draw background
    drawBackground();

    // Always update animations
    updateAnimations(deltaTime);

    // Draw current scene with new animations
    switch(currentScene) {
        case 0: // Mosquito Journey Scene
            if (!mosquitoes.empty()) {
                for (int i = 0; i < std::min(3, (int)mosquitoes.size()); ++i) {
                    drawMosquito(mosquitoes[i].x, mosquitoes[i].y, 0.5f, mosquitoes[i].wingAngle);
                }
            }
            drawMosquitoLifeCycle(-3.0f, 2.0f, 1.0f, mosquitoLifeCycle);
            drawHouse(-3.0f, -2.0f, 1.0f);
            break;

        case 1: // Clean Environment Scene
            drawHouse(-3.0f, 0.0f, 1.0f);
            drawHouse(3.0f, 0.0f, 1.0f);
            drawWaterContainer(0.0f, -2.0f, 1.0f, 0.0f);
            drawWaterRipple(0.0f, -2.0f, 1.0f, rippleTime);
            // Add cleaners
            for(const auto& person : people) {
                if(person.type == 1) {
                    drawPerson(person.x, person.y, person.scale, person.type, person.animationTime);
                }
            }
            break;

        case 2: // Dengue Fighter Scene
            // Add fighters
            for(const auto& person : people) {
                if(person.type == 0) {
                    drawPerson(person.x, person.y, person.scale, person.type, person.animationTime);
                }
            }
            drawAllMosquitoes();
            drawPulseEffect(0.0f, 0.0f, 1.0f);
            break;

        case 3: // Stagnant Water Scene
            drawWaterContainer(-3.0f, 1.0f, 1.0f, waterLevel);
            drawWaterContainer(0.0f, 1.0f, 1.0f, waterLevel);
            drawWaterContainer(3.0f, 1.0f, 1.0f, waterLevel);
            drawWaterRipple(-3.0f, 1.0f, 1.0f, rippleTime);
            drawWaterRipple(0.0f, 1.0f, 1.0f, rippleTime + 0.5f);
            drawWaterRipple(3.0f, 1.0f, 1.0f, rippleTime + 1.0f);
            drawAllMosquitoes();
            // Add inspectors
            for(const auto& person : people) {
                if(person.type == 2) {
                    drawPerson(person.x, person.y, person.scale, person.type, person.animationTime);
                }
            }
            break;

        case 4: // Home Inspection Scene
            drawHouse(-3.0f, 0.0f, 1.0f);
            // Add inspectors with spray
            for(const auto& person : people) {
                if(person.type == 2) {
                    drawPerson(person.x, person.y, person.scale, person.type, person.animationTime);
                }
            }
            drawWaterContainer(3.0f, -2.0f, 1.0f, waterLevel);
            break;

        case 5: // Symptoms Scene
            // Add patients
            for(const auto& person : people) {
                if(person.type == 3) {
                    drawPerson(person.x, person.y, person.scale, person.type, person.animationTime);
                }
            }
            break;

        case 6: // Prevention Methods Scene
            drawPerson(-3.0f, 0.0f, 1.0f, true, 1.0f);
            drawPerson(0.0f, 0.0f, 1.0f, true, 1.0f);
            drawPerson(3.0f, 0.0f, 1.0f, true, 1.0f);
            drawWaterContainer(0.0f, -2.0f, 1.0f, 0.0f);
            break;

        case 7: // Treatment Options Scene
            drawAmbulance(-2.0f, 0.0f, 1.0f);
            drawPerson(2.0f, 0.0f, 1.5f, false, 1.0f);
            break;

        case 8: // Statistics Scene
            drawStatistics();
            break;
    }

    // Draw rain effect
    drawRain();

    // Draw scene name box at top
    drawSceneNameBox();

    // Draw options box if toggled
    drawOptionsBox();

    // Draw emergency contacts and info messages
    drawEmergencyContacts();
    drawInfoMessages();

    // Force continuous redisplay
    glutPostRedisplay();
    glutSwapBuffers();
}

static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case '1': case '2': case '3': case '4': case '5':
        case '6': case '7': case '8': case '9':
            currentScene = key - '1';
            break;

        case 'm': case 'M':
            isAnimating = !isAnimating;
            break;

        case 'n': case 'N':
            isDayMode = !isDayMode;
            break;

        case 'd': case 'D':
            if(!isKillingMosquito) {
                // Find next alive mosquito
                for(size_t i = 0; i < mosquitoes.size(); i++) {
                    if(mosquitoes[i].alive) {
                        mosquitoToKill = i;
                        isKillingMosquito = true;
                        killEffectTime = 0.0f;
                        killedMosquitoes++;
                        break;
                    }
                }
            }
            break;

        case 'a': case 'A':
            showAmbulance = !showAmbulance;
            break;

        case 'e': case 'E':
            showEmergency = !showEmergency;
            break;

        case 'i': case 'I':
            showInfo = !showInfo;
            break;

        case 'o': case 'O':
            showOptions = !showOptions;
            break;

        case 'u': case 'U':
            showUpdates = !showUpdates;
            break;

        case 27: // ESC
            exit(0);
            break;
    }

    glutPostRedisplay();
}

static void mouse(int button, int state, int x, int y)
{
    // Convert window coordinates to OpenGL coordinates
    float glX = (x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 10.0f - 5.0f;
    float glY = 5.0f - (y / (float)glutGet(GLUT_WINDOW_HEIGHT)) * 10.0f;

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        // Check if any interactive element was clicked
        for(size_t i = 0; i < interactiveElements.size(); i++) {
            if(isPointInside(glX, glY, interactiveElements[i])) {
                selectedItem = i;
                break;
            }
        }
    }
}

static void motion(int x, int y)
{
    // Convert window coordinates to OpenGL coordinates
    float glX = (x / (float)glutGet(GLUT_WINDOW_WIDTH)) * 10.0f - 5.0f;
    float glY = 5.0f - (y / (float)glutGet(GLUT_WINDOW_HEIGHT)) * 10.0f;

    // Update hover state for interactive elements
    for(auto& element : interactiveElements) {
        element.isHovered = isPointInside(glX, glY, element);
    }
}

// Modify main function to initialize textures
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(800, 600);
    glutInitWindowPosition(10, 10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("Interactive Dengue Awareness");

    // Initialize textures
    initTextures();

    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Start automatic display
    startAutomaticDisplay();

    glutMainLoop();
    return EXIT_SUCCESS;
}
