# Interactive Dengue Disease Awareness Visualization

An educational visualization tool designed to raise awareness about dengue disease, particularly targeting young students and illiterate populations. This interactive application uses OpenGL to create engaging visual representations of dengue-related concepts, prevention methods, and statistics.

## Features

- **Interactive Scenes**: 9 different educational scenes covering various aspects of dengue
- **Real-time Animations**: Dynamic mosquito movements, water effects, and environmental changes
- **Day/Night Mode**: Toggle between day and night environments
- **Statistics Visualization**: Graphical representation of dengue cases and prevention measures
- **Educational Information**: Rotating information messages about dengue prevention and treatment
- **Interactive Elements**: Clickable objects and responsive animations
- **Multi-language Support**: Available in multiple languages for broader accessibility
- **Accessibility Features**: High contrast mode and screen reader support
- **Data Export**: Export statistics and educational content
- **Customization Options**: Adjustable interface elements and display settings

## Scenes

1. **Mosquito Journey Scene**: Shows the life cycle of mosquitoes
   - Egg stage visualization
   - Larva development
   - Pupa transformation
   - Adult mosquito emergence
   - Breeding behavior
   - Feeding patterns
   - Life span information

2. **Clean Environment Scene**: Demonstrates proper environmental maintenance
   - Proper waste disposal
   - Water container management
   - Garden maintenance
   - Drainage system care
   - Community cleanup procedures
   - Recycling guidelines
   - Environmental protection measures

3. **Dengue Fighter Scene**: Features community workers and prevention activities
   - Fogging operations
   - Larvicide application
   - Community education
   - Health worker training
   - Emergency response
   - Prevention campaigns
   - Volunteer activities

4. **Stagnant Water Scene**: Highlights common breeding grounds
   - Water container types
   - Common locations
   - Risk assessment
   - Prevention methods
   - Cleaning procedures
   - Monitoring techniques
   - Community guidelines

5. **Home Inspection Scene**: Shows proper home inspection procedures
   - Room-by-room inspection
   - Common hiding spots
   - Prevention measures
   - Maintenance schedules
   - Safety protocols
   - Documentation methods
   - Follow-up procedures

6. **Symptoms Scene**: Displays common dengue symptoms
   - Early warning signs
   - Severe symptoms
   - Emergency indicators
   - Treatment options
   - Recovery process
   - Prevention measures
   - Medical consultation guidelines

7. **Prevention Methods Scene**: Illustrates various prevention techniques
   - Personal protection
   - Environmental management
   - Community measures
   - Educational programs
   - Monitoring systems
   - Emergency protocols
   - Long-term strategies

8. **Treatment Options Scene**: Shows available treatment methods
   - Medical interventions
   - Home care
   - Hospital care
   - Emergency treatment
   - Recovery support
   - Follow-up care
   - Prevention after recovery

9. **Statistics Scene**: Presents dengue statistics from 2018-2023
   - Case numbers
   - Geographic distribution
   - Seasonal patterns
   - Risk factors
   - Prevention effectiveness
   - Treatment outcomes
   - Future projections

## Requirements

### System Requirements
- Windows/Linux/macOS operating system
- OpenGL compatible graphics card
- Minimum 2GB RAM
- 100MB free disk space
- Internet connection for updates
- Sound card for audio features
- Display resolution: 1280x720 minimum

### Development Requirements
- C++ compiler (GCC, MSVC, or Clang)
- OpenGL development libraries
- GLUT (OpenGL Utility Toolkit)
- CMake (optional, for building)
- Git for version control
- IDE (Visual Studio, Code::Blocks, or CLion)
- Debugging tools

## Installation

### Windows
1. Install MinGW or Visual Studio with C++ support
2. Install GLUT:
   - Download freeglut from [freeglut website](https://www.transmissionzero.co.uk/software/freeglut-devel/)
   - Extract and copy files to appropriate directories
   - Add to system PATH
3. Install additional dependencies:
   - GLM (OpenGL Mathematics)
   - SOIL (Simple OpenGL Image Library)
   - GLFW (OpenGL Framework)

### Linux
```bash
sudo apt-get update
sudo apt-get install freeglut3-dev
sudo apt-get install libglm-dev
sudo apt-get install libsoil-dev
sudo apt-get install libglfw3-dev
```

### macOS
```bash
brew install freeglut
brew install glm
brew install soil
brew install glfw
```

## Building the Project

### Using CMake
```bash
mkdir build
cd build
cmake ..
make
```

### Manual Compilation
```bash
g++ main.cpp -o dengue_awareness -lglut -lGL -lGLU -lglm -lsoil -lglfw
```

## Usage

### Controls

#### Scene Navigation
- **1**: Switch to Mosquito Journey Scene
  - Shows detailed life cycle
  - Interactive mosquito movements
  - Educational overlays
  - Zoom controls
  - Information panels

- **2**: Switch to Clean Environment Scene
  - Environmental maintenance
  - Waste management
  - Water container handling
  - Community guidelines
  - Best practices

- **3**: Switch to Dengue Fighter Scene
  - Community worker activities
  - Prevention measures
  - Emergency response
  - Training programs
  - Volunteer information

- **4**: Switch to Stagnant Water Scene
  - Breeding ground identification
  - Risk assessment
  - Prevention methods
  - Cleaning procedures
  - Monitoring techniques

- **5**: Switch to Home Inspection Scene
  - Room inspection
  - Safety protocols
  - Maintenance schedules
  - Documentation
  - Follow-up procedures

- **6**: Switch to Symptoms Scene
  - Warning signs
  - Emergency indicators
  - Treatment options
  - Recovery process
  - Medical guidelines

- **7**: Switch to Prevention Methods Scene
  - Personal protection
  - Environmental management
  - Community measures
  - Educational programs
  - Emergency protocols

- **8**: Switch to Treatment Options Scene
  - Medical interventions
  - Home care
  - Hospital care
  - Emergency treatment
  - Recovery support

- **9**: Switch to Statistics Scene
  - Case numbers
  - Geographic distribution
  - Seasonal patterns
  - Risk factors
  - Prevention effectiveness

#### Display Controls
- **M/m**: Toggle animation
  - Start/stop animations
  - Adjust animation speed
  - Control animation direction
  - Set animation preferences
  - Save animation settings

- **N/n**: Toggle day/night mode
  - Switch lighting
  - Adjust contrast
  - Change environment
  - Modify visibility
  - Update shadows

- **D/d**: Kill mosquitoes (interactive feature)
  - Target selection
  - Action confirmation
  - Success feedback
  - Statistics update
  - Achievement tracking

- **A/a**: Show/hide ambulance
  - Emergency response
  - Location tracking
  - Response time
  - Contact information
  - Service details

- **E/e**: Show/hide emergency contacts
  - Hospital numbers
  - Emergency services
  - Health centers
  - Support groups
  - Hotline information

- **I/i**: Show/hide information
  - Educational content
  - Statistics
  - Guidelines
  - Updates
  - Resources

- **O/o**: Show/hide options
  - Display settings
  - Audio controls
  - Language selection
  - Accessibility options
  - User preferences

- **U/u**: Show/hide updates
  - News
  - Statistics
  - Guidelines
  - Events
  - Resources

#### Additional Controls
- **ESC**: Exit application
- **F1**: Help menu
- **F2**: Settings menu
- **F3**: Statistics view
- **F4**: Fullscreen toggle
- **F5**: Refresh display
- **F6**: Save state
- **F7**: Load state
- **F8**: Export data
- **F9**: Import data
- **F10**: Print screen
- **F11**: Debug mode
- **F12**: Developer tools

#### Mouse Controls
- **Left Click**: Select/Interact
- **Right Click**: Context menu
- **Middle Click**: Pan view
- **Scroll**: Zoom in/out
- **Double Click**: Quick action

#### Touch Controls
- **Tap**: Select/Interact
- **Long Press**: Context menu
- **Swipe**: Navigate
- **Pinch**: Zoom
- **Rotate**: Rotate view

### Running the Application
```bash
./dengue_awareness
```

## Features in Detail

### Interactive Elements
- Click on objects to learn more about them
- Watch real-time animations of mosquito life cycles
- Observe water level changes in containers
- View statistics with animated graphs
- Interact with educational content
- Participate in virtual scenarios
- Track progress and achievements

### Educational Content
- Rotating information messages
- Visual demonstrations of prevention methods
- Statistical data visualization
- Emergency contact information
- Interactive tutorials
- Case studies
- Expert interviews
- Research findings

### Accessibility Features
- Screen reader support
- High contrast mode
- Text size adjustment
- Color blind mode
- Keyboard navigation
- Voice commands
- Subtitles
- Audio descriptions

### Data Management
- Export statistics
- Save progress
- Load previous sessions
- Backup data
- Share information
- Generate reports
- Track usage
- Monitor performance

## Contributing

### How to Contribute
1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Submit a pull request
5. Follow coding guidelines
6. Add documentation
7. Include tests
8. Update changelog

### Development Guidelines
- Follow C++ best practices
- Use consistent formatting
- Write clear comments
- Create unit tests
- Update documentation
- Review code
- Test thoroughly
- Maintain compatibility

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- OpenGL community for graphics support
- FreeGLUT developers for the utility toolkit
- Health organizations for dengue-related information
- Contributors and maintainers
- Testing volunteers
- Educational institutions
- Healthcare professionals
- Community organizations

## Support

### Getting Help
- GitHub issues
- Documentation
- Community forums
- Email support
- Live chat
- Social media
- FAQ section
- Tutorial videos

### Reporting Issues
- Bug reports
- Feature requests
- Documentation updates
- Performance issues
- Security concerns
- Compatibility problems
- User experience feedback
- Accessibility improvements

## Future Enhancements

### Planned Features
- Multi-language support
- Mobile application version
- Additional interactive scenarios
- Real-time data integration
- Virtual reality support
- Augmented reality features
- Machine learning integration
- Cloud synchronization

### Technical Improvements
- Performance optimization
- Code refactoring
- Modern OpenGL support
- Cross-platform compatibility
- Mobile responsiveness
- Offline functionality
- Data security
- User authentication

---

*This project is designed for educational purposes and should not be used as a substitute for professional medical advice.* 