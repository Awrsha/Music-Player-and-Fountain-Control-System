import 'dart:async';
import 'dart:math';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'FountainPlayer',
      debugShowCheckedModeBanner: false,
      themeMode: ThemeMode.system,
      theme: ThemeData(
        primaryColor: const Color(0xFF3B82F6),
        primarySwatch: Colors.blue,
        fontFamily: 'Inter',
        scaffoldBackgroundColor: const Color(0xFFF9FAFB),
        cardTheme: CardTheme(
          shape: RoundedRectangleBorder(borderRadius: BorderRadius.circular(12)),
          elevation: 1,
        ),
        appBarTheme: const AppBarTheme(
          backgroundColor: Colors.white,
          foregroundColor: Color(0xFF1F2937),
          elevation: 1,
          systemOverlayStyle: SystemUiOverlayStyle.dark,
        ),
      ),
      darkTheme: ThemeData.dark().copyWith(
        primaryColor: const Color(0xFF3B82F6),
        scaffoldBackgroundColor: const Color(0xFF121218),
        cardColor: const Color(0xFF1E1E24),
        canvasColor: const Color(0xFF1E1E24),
        appBarTheme: const AppBarTheme(
          backgroundColor: Color(0xFF1E1E24),
          foregroundColor: Colors.white,
          elevation: 1,
          systemOverlayStyle: SystemUiOverlayStyle.light,
        ),
      ),
      home: const FountainPlayerHome(),
    );
  }
}

class FountainPlayerHome extends StatefulWidget {
  const FountainPlayerHome({Key? key}) : super(key: key);

  @override
  _FountainPlayerHomeState createState() => _FountainPlayerHomeState();
}

class _FountainPlayerHomeState extends State<FountainPlayerHome> with SingleTickerProviderStateMixin {
  final GlobalKey<ScaffoldState> _scaffoldKey = GlobalKey<ScaffoldState>();
  int _selectedIndex = 0;
  bool _isPlaying = true;
  double _currentVolume = 75;
  double _intensityValue = 75;
  double _speedValue = 50;
  String _selectedFountainPattern = "Wave (Synchronized)";
  String _selectedSyncMode = "Beat Detection";
  int _selectedColor = 0;
  final List<Color> _colors = [
    const Color(0xFF3B82F6),
    const Color(0xFF8B5CF6),
    const Color(0xFFEF4444),
    const Color(0xFF10B981),
    const Color(0xFFF59E0B),
  ];
  
  Timer? _visualizerTimer;
  List<double> _visualizerHeights = [];
  
  late AnimationController _animationController;
  
  @override
  void initState() {
    super.initState();
    
    _animationController = AnimationController(
      vsync: this,
      duration: const Duration(milliseconds: 100),
    );
    
    _visualizerHeights = List.generate(38, (_) => _getRandomHeight());
    
    _startVisualizerAnimation();
  }
  
  @override
  void dispose() {
    _visualizerTimer?.cancel();
    _animationController.dispose();
    super.dispose();
  }
  
  double _getRandomHeight() {
    return 0.2 + Random().nextDouble() * 0.8;
  }
  
  void _startVisualizerAnimation() {
    _visualizerTimer = Timer.periodic(const Duration(milliseconds: 100), (_) {
      if (mounted) {
        setState(() {
          _visualizerHeights = List.generate(38, (_) => _getRandomHeight());
        });
      }
    });
  }
  
  void _togglePlayPause() {
    setState(() {
      _isPlaying = !_isPlaying;
    });
  }
  
  void _updateVolume(double value) {
    setState(() {
      _currentVolume = value;
    });
  }
  
  void _updateIntensity(double value) {
    setState(() {
      _intensityValue = value;
    });
  }
  
  void _updateSpeed(double value) {
    setState(() {
      _speedValue = value;
    });
  }
  
  void _changePattern(String? pattern) {
    if (pattern != null) {
      setState(() {
        _selectedFountainPattern = pattern;
      });
    }
  }
  
  void _changeSyncMode(String? mode) {
    if (mode != null) {
      setState(() {
        _selectedSyncMode = mode;
      });
    }
  }
  
  void _selectColor(int index) {
    setState(() {
      _selectedColor = index;
    });
  }
  
  @override
  Widget build(BuildContext context) {
    final isDarkMode = Theme.of(context).brightness == Brightness.dark;
    final primaryColor = Theme.of(context).primaryColor;
    final secondaryColor = const Color(0xFF8B5CF6);
    
    return Scaffold(
      key: _scaffoldKey,
      appBar: AppBar(
        title: const Text('Dashboard', style: TextStyle(fontWeight: FontWeight.bold)),
        leading: IconButton(
          icon: const Icon(Icons.menu),
          onPressed: () {
            _scaffoldKey.currentState?.openDrawer();
          },
        ),
        actions: [
          IconButton(
            icon: const Icon(Icons.notifications_outlined),
            onPressed: () {},
          ),
          const SizedBox(width: 8),
          CircleAvatar(
            radius: 16,
            backgroundColor: isDarkMode ? Colors.grey[700] : Colors.grey[200],
            child: Icon(Icons.person_outline, color: isDarkMode ? Colors.grey[300] : Colors.grey[600]),
          ),
          const SizedBox(width: 16),
        ],
      ),
      drawer: Drawer(
        child: Column(
          children: [
            DrawerHeader(
              decoration: BoxDecoration(
                color: isDarkMode ? const Color(0xFF1E1E24) : Colors.white,
              ),
              child: Row(
                children: [
                  Container(
                    width: 40,
                    height: 40,
                    decoration: BoxDecoration(
                      color: primaryColor,
                      borderRadius: BorderRadius.circular(10),
                    ),
                    child: const Icon(Icons.music_note, color: Colors.white),
                  ),
                  const SizedBox(width: 16),
                  const Text('FountainPlayer', style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold)),
                ],
              ),
            ),
            ListTile(
              selected: _selectedIndex == 0,
              selectedTileColor: isDarkMode ? primaryColor.withOpacity(0.2) : primaryColor.withOpacity(0.1),
              leading: const Icon(Icons.home_outlined),
              title: const Text('Home'),
              onTap: () {
                setState(() {
                  _selectedIndex = 0;
                });
                Navigator.pop(context);
              },
            ),
            ListTile(
              selected: _selectedIndex == 1,
              selectedTileColor: isDarkMode ? primaryColor.withOpacity(0.2) : primaryColor.withOpacity(0.1),
              leading: const Icon(Icons.music_note_outlined),
              title: const Text('Music'),
              onTap: () {
                setState(() {
                  _selectedIndex = 1;
                });
                Navigator.pop(context);
              },
            ),
            ListTile(
              selected: _selectedIndex == 2,
              selectedTileColor: isDarkMode ? primaryColor.withOpacity(0.2) : primaryColor.withOpacity(0.1),
              leading: const Icon(Icons.water_drop_outlined),
              title: const Text('Fountain'),
              onTap: () {
                setState(() {
                  _selectedIndex = 2;
                });
                Navigator.pop(context);
              },
            ),
            ListTile(
              selected: _selectedIndex == 3,
              selectedTileColor: isDarkMode ? primaryColor.withOpacity(0.2) : primaryColor.withOpacity(0.1),
              leading: const Icon(Icons.settings_outlined),
              title: const Text('Settings'),
              onTap: () {
                setState(() {
                  _selectedIndex = 3;
                });
                Navigator.pop(context);
              },
            ),
            ListTile(
              selected: _selectedIndex == 4,
              selectedTileColor: isDarkMode ? primaryColor.withOpacity(0.2) : primaryColor.withOpacity(0.1),
              leading: const Icon(Icons.info_outline),
              title: const Text('About'),
              onTap: () {
                setState(() {
                  _selectedIndex = 4;
                });
                Navigator.pop(context);
              },
            ),
            const Spacer(),
            Container(
              margin: const EdgeInsets.all(16),
              padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
              decoration: BoxDecoration(
                color: isDarkMode ? Colors.grey[800] : Colors.grey[100],
                borderRadius: BorderRadius.circular(10),
              ),
              child: Row(
                children: [
                  Container(
                    width: 8,
                    height: 8,
                    decoration: BoxDecoration(
                      color: Colors.green,
                      borderRadius: BorderRadius.circular(4),
                      boxShadow: [
                        BoxShadow(
                          color: Colors.green.withOpacity(0.5),
                          blurRadius: 4,
                          spreadRadius: 1,
                        )
                      ],
                    ),
                  ),
                  const SizedBox(width: 8),
                  const Text('System Online', style: TextStyle(fontSize: 14)),
                ],
              ),
            ),
            Padding(
              padding: const EdgeInsets.all(16),
              child: InkWell(
                borderRadius: BorderRadius.circular(10),
                onTap: () {
                  final currentBrightness = Theme.of(context).brightness;
                  if (currentBrightness == Brightness.dark) {
                    // Switch to light theme
                  } else {
                    // Switch to dark theme
                  }
                },
                child: Container(
                  padding: const EdgeInsets.symmetric(horizontal: 16, vertical: 12),
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(10),
                  ),
                  child: Row(
                    children: [
                      Icon(
                        isDarkMode ? Icons.dark_mode_outlined : Icons.light_mode_outlined,
                        color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                      ),
                      const SizedBox(width: 16),
                      const Text('Toggle Theme'),
                    ],
                  ),
                ),
              ),
            ),
          ],
        ),
      ),
      body: SingleChildScrollView(
        child: Padding(
          padding: const EdgeInsets.all(16.0),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              // Overview Cards
              GridView.count(
                crossAxisCount: 2,
                shrinkWrap: true,
                physics: const NeverScrollableScrollPhysics(),
                childAspectRatio: 3.0,
                crossAxisSpacing: 16,
                mainAxisSpacing: 16,
                children: [
                  _buildStatCard(
                    icon: Icons.music_note,
                    iconColor: primaryColor,
                    backgroundColor: isDarkMode ? primaryColor.withOpacity(0.2) : primaryColor.withOpacity(0.1),
                    title: 'Tracks',
                    value: '256',
                  ),
                  _buildStatCard(
                    icon: Icons.grid_view,
                    iconColor: secondaryColor,
                    backgroundColor: isDarkMode ? secondaryColor.withOpacity(0.2) : secondaryColor.withOpacity(0.1),
                    title: 'Playlists',
                    value: '14',
                  ),
                  _buildStatCard(
                    icon: Icons.flash_on,
                    iconColor: Colors.green,
                    backgroundColor: isDarkMode ? Colors.green.withOpacity(0.2) : Colors.green.withOpacity(0.1),
                    title: 'Patterns',
                    value: '8',
                  ),
                  _buildStatCard(
                    icon: Icons.schedule,
                    iconColor: Colors.purple,
                    backgroundColor: isDarkMode ? Colors.purple.withOpacity(0.2) : Colors.purple.withOpacity(0.1),
                    title: 'Play Time',
                    value: '28h',
                  ),
                ],
              ),
              
              const SizedBox(height: 24),
              
              // Now Playing and Fountain Visualization
              Row(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Expanded(
                    child: _buildNowPlayingCard(isDarkMode, primaryColor),
                  ),
                  const SizedBox(width: 16),
                  Expanded(
                    child: _buildFountainControlCard(isDarkMode, primaryColor),
                  ),
                ],
              ),
              
              const SizedBox(height: 24),
              
              // Playlist and DMX Settings
              Row(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Expanded(
                    child: _buildPlaylistCard(isDarkMode, primaryColor),
                  ),
                  const SizedBox(width: 16),
                  Expanded(
                    child: _buildDMXConfigCard(isDarkMode, primaryColor),
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
      bottomNavigationBar: _buildPlayerControlBar(isDarkMode, primaryColor),
    );
  }
  
  Widget _buildStatCard({
    required IconData icon,
    required Color iconColor,
    required Color backgroundColor,
    required String title,
    required String value,
  }) {
    return Card(
      child: Padding(
        padding: const EdgeInsets.all(16),
        child: Row(
          children: [
            Container(
              width: 48,
              height: 48,
              decoration: BoxDecoration(
                color: backgroundColor,
                borderRadius: BorderRadius.circular(10),
              ),
              child: Icon(icon, color: iconColor),
            ),
            const SizedBox(width: 16),
            Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                Text(
                  title,
                  style: TextStyle(
                    color: Theme.of(context).brightness == Brightness.dark 
                        ? Colors.grey[400] 
                        : Colors.grey[600],
                    fontSize: 12,
                  ),
                ),
                Text(
                  value,
                  style: const TextStyle(fontSize: 20, fontWeight: FontWeight.bold),
                ),
              ],
            ),
          ],
        ),
      ),
    );
  }
  
  Widget _buildNowPlayingCard(bool isDarkMode, Color primaryColor) {
    return Card(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Padding(
            padding: const EdgeInsets.all(16),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                const Text('Now Playing', style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
                Row(
                  children: [
                    Container(
                      width: 8,
                      height: 8,
                      decoration: BoxDecoration(
                        color: Colors.green,
                        borderRadius: BorderRadius.circular(4),
                        boxShadow: [
                          BoxShadow(
                            color: Colors.green.withOpacity(0.5),
                            blurRadius: 4,
                            spreadRadius: 1,
                          )
                        ],
                      ),
                    ),
                    const SizedBox(width: 8),
                    Text(
                      'Playing',
                      style: TextStyle(
                        fontSize: 12,
                        color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),
          Divider(height: 1, color: isDarkMode ? Colors.grey[800] : Colors.grey[200]),
          Padding(
            padding: const EdgeInsets.all(16),
            child: Row(
              children: [
                Container(
                  width: 80,
                  height: 80,
                  decoration: BoxDecoration(
                    borderRadius: BorderRadius.circular(10),
                    gradient: LinearGradient(
                      begin: Alignment.topLeft,
                      end: Alignment.bottomRight,
                      colors: [primaryColor, const Color(0xFF8B5CF6)],
                    ),
                  ),
                  child: const Icon(Icons.music_note, color: Colors.white, size: 32),
                ),
                const SizedBox(width: 16),
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      const Text('Summer Vibes', style: TextStyle(fontSize: 20, fontWeight: FontWeight.bold)),
                      Text('Electronic Beats', style: TextStyle(color: isDarkMode ? Colors.grey[400] : Colors.grey[600])),
                      Text(
                        'From: Summer Playlist',
                        style: TextStyle(
                          fontSize: 12,
                          color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                        ),
                      ),
                      const SizedBox(height: 12),
                      ClipRRect(
                        borderRadius: BorderRadius.circular(2),
                        child: LinearProgressIndicator(
                          value: 0.45,
                          backgroundColor: isDarkMode ? Colors.grey[800] : Colors.grey[200],
                          valueColor: AlwaysStoppedAnimation<Color>(primaryColor),
                        ),
                      ),
                      const SizedBox(height: 4),
                      Row(
                        mainAxisAlignment: MainAxisAlignment.spaceBetween,
                        children: [
                          Text(
                            '1:45',
                            style: TextStyle(
                              fontSize: 10,
                              color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                            ),
                          ),
                          Text(
                            '3:52',
                            style: TextStyle(
                              fontSize: 10,
                              color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                            ),
                          ),
                        ],
                      ),
                    ],
                  ),
                ),
              ],
            ),
          ),
          Padding(
            padding: const EdgeInsets.symmetric(horizontal: 24, vertical: 16),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                IconButton(
                  icon: const Icon(Icons.fast_rewind),
                  onPressed: () {},
                  color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                ),
                const SizedBox(width: 16),
                FloatingActionButton(
                  backgroundColor: primaryColor,
                  onPressed: _togglePlayPause,
                  child: Icon(_isPlaying ? Icons.pause : Icons.play_arrow),
                ),
                const SizedBox(width: 16),
                IconButton(
                  icon: const Icon(Icons.fast_forward),
                  onPressed: () {},
                  color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                ),
              ],
            ),
          ),
          Padding(
            padding: const EdgeInsets.all(16),
            child: SizedBox(
              height: 60,
              child: Row(
                children: List.generate(
                  _visualizerHeights.length,
                  (index) => Expanded(
                    child: Padding(
                      padding: const EdgeInsets.symmetric(horizontal: 1),
                      child: Container(
                        height: 60 * _visualizerHeights[index],
                        decoration: BoxDecoration(
                          color: primaryColor,
                          borderRadius: BorderRadius.circular(2),
                        ),
                      ),
                    ),
                  ),
                ),
              ),
            ),
          ),
          Padding(
            padding: const EdgeInsets.all(16),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                IconButton(
                  icon: const Icon(Icons.mic_outlined),
                  onPressed: () {},
                  color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                ),
                Expanded(
                  child: Slider(
                    value: _currentVolume,
                    min: 0,
                    max: 100,
                    activeColor: primaryColor,
                    inactiveColor: isDarkMode ? Colors.grey[800] : Colors.grey[200],
                    onChanged: _updateVolume,
                  ),
                ),
                IconButton(
                  icon: const Icon(Icons.refresh),
                  onPressed: () {},
                  color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
  
  Widget _buildFountainControlCard(bool isDarkMode, Color primaryColor) {
    return Card(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Padding(
            padding: const EdgeInsets.all(16),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                const Text('Fountain Control', style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
                Row(
                  children: [
                    Text(
                      'Pattern:',
                      style: TextStyle(
                        fontSize: 12,
                        color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                      ),
                    ),
                    const SizedBox(width: 8),
                    DropdownButton<String>(
                      value: _selectedFountainPattern,
                      underline: Container(height: 0),
                      isDense: true,
                      items: [
                        'Wave (Synchronized)',
                        'Burst',
                        'Cascade',
                        'Random',
                      ].map((String value) {
                        return DropdownMenuItem<String>(
                          value: value,
                          child: Text(value, style: const TextStyle(fontSize: 12)),
                        );
                      }).toList(),
                      onChanged: _changePattern,
                    ),
                  ],
                ),
              ],
            ),
          ),
          Divider(height: 1, color: isDarkMode ? Colors.grey[800] : Colors.grey[200]),
          Padding(
            padding: const EdgeInsets.all(16),
            child: Column(
              children: [
                Container(
                  height: 200,
                  decoration: BoxDecoration(
                    color: isDarkMode ? Colors.blue.withOpacity(0.1) : Colors.blue.withOpacity(0.05),
                    borderRadius: BorderRadius.circular(12),
                  ),
                  child: Stack(
                    children: [
                      for (int i = 0; i < 17; i++)
                        Positioned(
                          left: (i * 5 + 10).toDouble() + (i * 1.5),
                          bottom: 0,
                          child: Container(
                            width: 4,
                            height: 80 + (i % 3) * 20.0 + (Random().nextDouble() * 20),
                            decoration: BoxDecoration(
                              gradient: LinearGradient(
                                begin: Alignment.bottomCenter,
                                end: Alignment.topCenter,
                                colors: [
                                  _colors[_selectedColor].withOpacity(0.7),
                                  _colors[_selectedColor].withOpacity(0.2),
                                ],
                              ),
                              borderRadius: BorderRadius.circular(4),
                            ),
                          ),
                        ),
                      Positioned(
                        bottom: 0,
                        left: 0,
                        right: 0,
                        child: Container(
                          height: 5,
                          decoration: BoxDecoration(
                            color: isDarkMode
                                ? primaryColor.withOpacity(0.3)
                                : primaryColor.withOpacity(0.1),
                          ),
                        ),
                      ),
                    ],
                  ),
                ),
                const SizedBox(height: 16),
                Row(
                  children: [
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Intensity',
                            style: TextStyle(
                              fontWeight: FontWeight.w500,
                              color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                            ),
                          ),
                          Slider(
                            value: _intensityValue,
                            min: 0,
                            max: 100,
                            activeColor: primaryColor,
                            inactiveColor: isDarkMode ? Colors.grey[800] : Colors.grey[200],
                            onChanged: _updateIntensity,
                          ),
                          Row(
                            mainAxisAlignment: MainAxisAlignment.spaceBetween,
                            children: [
                              Text(
                                'Low',
                                style: TextStyle(
                                  fontSize: 10,
                                  color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                                ),
                              ),
                              Text(
                                'High',
                                style: TextStyle(
                                  fontSize: 10,
                                  color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                                ),
                              ),
                            ],
                          ),
                        ],
                      ),
                    ),
                    const SizedBox(width: 16),
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Speed',
                            style: TextStyle(
                              fontWeight: FontWeight.w500,
                              color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                            ),
                          ),
                          Slider(
                            value: _speedValue,
                            min: 0,
                            max: 100,
                            activeColor: primaryColor,
                            inactiveColor: isDarkMode ? Colors.grey[800] : Colors.grey[200],
                            onChanged: _updateSpeed,
                          ),
                          Row(
                            mainAxisAlignment: MainAxisAlignment.spaceBetween,
                            children: [
                              Text(
                                'Slow',
                                style: TextStyle(
                                  fontSize: 10,
                                  color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                                ),
                              ),
                              Text(
                                'Fast',
                                style: TextStyle(
                                  fontSize: 10,
                                  color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                                ),
                              ),
                            ],
                          ),
                        ],
                      ),
                    ),
                  ],
                ),
                const SizedBox(height: 16),
                Row(
                  children: [
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Color',
                            style: TextStyle(
                              fontWeight: FontWeight.w500,
                              color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                            ),
                          ),
                          const SizedBox(height: 8),
                          Row(
                            children: List.generate(
                              _colors.length + 1,
                              (index) {
                                if (index < _colors.length) {
                                  return Padding(
                                    padding: const EdgeInsets.only(right: 8),
                                    child: InkWell(
                                      onTap: () => _selectColor(index),
                                      child: Container(
                                        width: 24,
                                        height: 24,
                                        decoration: BoxDecoration(
                                          color: _colors[index],
                                          borderRadius: BorderRadius.circular(12),
                                          border: _selectedColor == index
                                              ? Border.all(
                                                  color: Colors.white,
                                                  width: 2,
                                                )
                                              : null,
                                          boxShadow: _selectedColor == index
                                              ? [
                                                  BoxShadow(
                                                    color: _colors[index].withOpacity(0.5),
                                                    spreadRadius: 2,
                                                    blurRadius: 4,
                                                  ),
                                                ]
                                              : null,
                                        ),
                                      ),
                                    ),
                                  );
                                } else {
                                  return Padding(
                                    padding: const EdgeInsets.only(right: 8),
                                    child: InkWell(
                                      onTap: () => _selectColor(index),
                                      child: Container(
                                        width: 24,
                                        height: 24,
                                        decoration: BoxDecoration(
                                          gradient: const LinearGradient(
                                            colors: [Color(0xFF3B82F6), Color(0xFF8B5CF6)],
                                            begin: Alignment.topLeft,
                                            end: Alignment.bottomRight,
                                          ),
                                          borderRadius: BorderRadius.circular(12),
                                          border: _selectedColor == index
                                              ? Border.all(
                                                  color: Colors.white,
                                                  width: 2,
                                                )
                                              : null,
                                          boxShadow: _selectedColor == index
                                              ? [
                                                  BoxShadow(
                                                    color: Colors.blue.withOpacity(0.5),
                                                    spreadRadius: 2,
                                                    blurRadius: 4,
                                                  ),
                                                ]
                                              : null,
                                        ),
                                      ),
                                    ),
                                  );
                                }
                              },
                            ),
                          ),
                        ],
                      ),
                    ),
                    const SizedBox(width: 16),
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Sync Mode',
                            style: TextStyle(
                              fontWeight: FontWeight.w500,
                              color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                            ),
                          ),
                          const SizedBox(height: 8),
                          Container(
                            padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
                            decoration: BoxDecoration(
                              color: isDarkMode ? Colors.grey[800] : Colors.grey[100],
                              borderRadius: BorderRadius.circular(8),
                              border: Border.all(
                                color: isDarkMode ? Colors.grey[700]! : Colors.grey[300]!,
                              ),
                            ),
                            child: DropdownButton<String>(
                              value: _selectedSyncMode,
                              isExpanded: true,
                              underline: Container(height: 0),
                              icon: const Icon(Icons.keyboard_arrow_down),
                              items: [
                                'Beat Detection',
                                'Frequency',
                                'Manual',
                                'Random',
                              ].map((String value) {
                                return DropdownMenuItem<String>(
                                  value: value,
                                  child: Text(value),
                                );
                              }).toList(),
                              onChanged: _changeSyncMode,
                            ),
                          ),
                        ],
                      ),
                    ),
                  ],
                ),
                const SizedBox(height: 16),
                Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    Row(
                      children: [
                        Container(
                          width: 8,
                          height: 8,
                          decoration: BoxDecoration(
                            color: Colors.green,
                            borderRadius: BorderRadius.circular(4),
                            boxShadow: [
                              BoxShadow(
                                color: Colors.green.withOpacity(0.5),
                                blurRadius: 4,
                                spreadRadius: 1,
                              )
                            ],
                          ),
                        ),
                        const SizedBox(width: 8),
                        Text(
                          'DMX Connected',
                          style: TextStyle(
                            fontSize: 12,
                            color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                          ),
                        ),
                      ],
                    ),
                    ElevatedButton.icon(
                      onPressed: () {},
                      icon: const Icon(Icons.mood),
                      label: const Text('Test Fountain'),
                      style: ElevatedButton.styleFrom(
                        backgroundColor: primaryColor,
                        foregroundColor: Colors.white,
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
  
  Widget _buildPlaylistCard(bool isDarkMode, Color primaryColor) {
    return Card(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Padding(
            padding: const EdgeInsets.all(16),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                const Text('Current Playlist', style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
                ElevatedButton.icon(
                  onPressed: () {},
                  icon: const Icon(Icons.add, size: 16),
                  label: const Text('Add Track'),
                  style: ElevatedButton.styleFrom(
                    backgroundColor: isDarkMode ? primaryColor.withOpacity(0.2) : primaryColor.withOpacity(0.1),
                    foregroundColor: isDarkMode ? primaryColor.withOpacity(0.8) : primaryColor,
                    elevation: 0,
                    padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 8),
                    textStyle: const TextStyle(fontSize: 12),
                  ),
                ),
              ],
            ),
          ),
          Divider(height: 1, color: isDarkMode ? Colors.grey[800] : Colors.grey[200]),
          SizedBox(
            height: 280,
            child: ListView(
              padding: const EdgeInsets.all(16),
              children: [
                _buildTrackItem(
                  isDarkMode,
                  primaryColor,
                  title: 'Summer Vibes',
                  subtitle: 'Electronic Beats • 3:52',
                  isPlaying: true,
                ),
                const SizedBox(height: 8),
                _buildTrackItem(
                  isDarkMode,
                  primaryColor,
                  title: 'Ocean Waves',
                  subtitle: 'Ambient Sound • 5:17',
                ),
                const SizedBox(height: 8),
                _buildTrackItem(
                  isDarkMode,
                  primaryColor,
                  title: 'Moonlight Sonata',
                  subtitle: 'Classical • 4:35',
                ),
                const SizedBox(height: 8),
                _buildTrackItem(
                  isDarkMode,
                  primaryColor,
                  title: 'Dance Floor',
                  subtitle: 'Disco • 3:18',
                ),
                const SizedBox(height: 8),
                _buildTrackItem(
                  isDarkMode,
                  primaryColor,
                  title: 'Fountain Symphony',
                  subtitle: 'Orchestral • 4:52',
                ),
              ],
            ),
          ),
          Divider(height: 1, color: isDarkMode ? Colors.grey[800] : Colors.grey[200]),
          Padding(
            padding: const EdgeInsets.all(16),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Row(
                  children: [
                    IconButton(
                      icon: const Icon(Icons.refresh),
                      onPressed: () {},
                      color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                    ),
                    IconButton(
                      icon: const Icon(Icons.swap_vert),
                      onPressed: () {},
                      color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                    ),
                    IconButton(
                      icon: const Icon(Icons.stop),
                      onPressed: () {},
                      color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                    ),
                  ],
                ),
                Container(
                  padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
                  decoration: BoxDecoration(
                    color: isDarkMode ? Colors.grey[800] : Colors.grey[100],
                    borderRadius: BorderRadius.circular(8),
                    border: Border.all(
                      color: isDarkMode ? Colors.grey[700]! : Colors.grey[300]!,
                    ),
                  ),
                  child: DropdownButton<String>(
                    value: 'Default Mode',
                    underline: Container(height: 0),
                    icon: const Icon(Icons.keyboard_arrow_down),
                    items: [
                      'Default Mode',
                      'Repeat All',
                      'Repeat One',
                      'Shuffle',
                    ].map((String value) {
                      return DropdownMenuItem<String>(
                        value: value,
                        child: Text(value),
                      );
                    }).toList(),
                    onChanged: (String? value) {},
                  ),
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
  
  Widget _buildTrackItem(
    bool isDarkMode,
    Color primaryColor, {
    required String title,
    required String subtitle,
    bool isPlaying = false,
  }) {
    return Container(
      decoration: BoxDecoration(
        color: isPlaying
            ? (isDarkMode ? primaryColor.withOpacity(0.2) : primaryColor.withOpacity(0.05))
            : Colors.transparent,
        borderRadius: BorderRadius.circular(8),
      ),
      child: Padding(
        padding: const EdgeInsets.all(8.0),
        child: Row(
          children: [
            Container(
              width: 40,
              height: 40,
              decoration: BoxDecoration(
                color: isPlaying
                    ? (isDarkMode ? primaryColor.withOpacity(0.3) : primaryColor.withOpacity(0.1))
                    : (isDarkMode ? Colors.grey[800] : Colors.grey[100]),
                borderRadius: BorderRadius.circular(8),
              ),
              child: Icon(
                isPlaying ? Icons.play_circle_filled : Icons.music_note,
                color: isPlaying ? primaryColor : (isDarkMode ? Colors.grey[400] : Colors.grey[600]),
                size: 20,
              ),
            ),
            const SizedBox(width: 12),
            Expanded(
              child: Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    title,
                    style: TextStyle(
                      fontWeight: FontWeight.w500,
                      color: isPlaying ? primaryColor : null,
                    ),
                  ),
                  Text(
                    subtitle,
                    style: TextStyle(
                      fontSize: 12,
                      color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                    ),
                  ),
                ],
              ),
            ),
            Container(
              width: 32,
              height: 32,
              decoration: BoxDecoration(
                color: isPlaying ? primaryColor : Colors.transparent,
                borderRadius: BorderRadius.circular(16),
              ),
              child: Icon(
                isPlaying ? Icons.pause : Icons.play_arrow,
                color: isPlaying ? Colors.white : (isDarkMode ? Colors.grey[400] : Colors.grey[600]),
                size: 18,
              ),
            ),
          ],
        ),
      ),
    );
  }
  
  Widget _buildDMXConfigCard(bool isDarkMode, Color primaryColor) {
    final secondaryColor = const Color(0xFF8B5CF6);
    
    return Card(
      child: Column(
        crossAxisAlignment: CrossAxisAlignment.start,
        children: [
          Padding(
            padding: const EdgeInsets.all(16),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                const Text('DMX Configuration', style: TextStyle(fontSize: 18, fontWeight: FontWeight.bold)),
                ElevatedButton.icon(
                  onPressed: () {},
                  icon: const Icon(Icons.check, size: 16),
                  label: const Text('Save Config'),
                  style: ElevatedButton.styleFrom(
                    backgroundColor: isDarkMode ? Colors.green.withOpacity(0.2) : Colors.green.withOpacity(0.1),
                    foregroundColor: isDarkMode ? Colors.green.withOpacity(0.8) : Colors.green,
                    elevation: 0,
                    padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 8),
                    textStyle: const TextStyle(fontSize: 12),
                  ),
                ),
              ],
            ),
          ),
          Divider(height: 1, color: isDarkMode ? Colors.grey[800] : Colors.grey[200]),
          Padding(
            padding: const EdgeInsets.all(16),
            child: Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              children: [
                Row(
                  children: [
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Interface',
                            style: TextStyle(
                              fontWeight: FontWeight.w500,
                              color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                              fontSize: 12,
                            ),
                          ),
                          const SizedBox(height: 8),
                          Container(
                            padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
                            decoration: BoxDecoration(
                              color: isDarkMode ? Colors.grey[800] : Colors.grey[100],
                              borderRadius: BorderRadius.circular(8),
                              border: Border.all(
                                color: isDarkMode ? Colors.grey[700]! : Colors.grey[300]!,
                              ),
                            ),
                            child: DropdownButton<String>(
                              value: 'Raspberry Pi GPIO',
                              isExpanded: true,
                              underline: Container(height: 0),
                              icon: const Icon(Icons.keyboard_arrow_down),
                              items: [
                                'Raspberry Pi GPIO',
                                'USB DMX Interface',
                                'Ethernet DMX',
                                'Custom Hardware',
                              ].map((String value) {
                                return DropdownMenuItem<String>(
                                  value: value,
                                  child: Text(value),
                                );
                              }).toList(),
                              onChanged: (String? value) {},
                            ),
                          ),
                        ],
                      ),
                    ),
                    const SizedBox(width: 16),
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Channel Mode',
                            style: TextStyle(
                              fontWeight: FontWeight.w500,
                              color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                              fontSize: 12,
                            ),
                          ),
                          const SizedBox(height: 8),
                          Container(
                            padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
                            decoration: BoxDecoration(
                              color: isDarkMode ? Colors.grey[800] : Colors.grey[100],
                              borderRadius: BorderRadius.circular(8),
                              border: Border.all(
                                color: isDarkMode ? Colors.grey[700]! : Colors.grey[300]!,
                              ),
                            ),
                            child: DropdownButton<String>(
                              value: '16 Channels',
                              isExpanded: true,
                              underline: Container(height: 0),
                              icon: const Icon(Icons.keyboard_arrow_down),
                              items: [
                                '16 Channels',
                                '32 Channels',
                                '64 Channels',
                                '128 Channels',
                              ].map((String value) {
                                return DropdownMenuItem<String>(
                                  value: value,
                                  child: Text(value),
                                );
                              }).toList(),
                              onChanged: (String? value) {},
                            ),
                          ),
                        ],
                      ),
                    ),
                  ],
                ),
                const SizedBox(height: 24),
                Text(
                  'DMX Channels',
                  style: TextStyle(
                    fontWeight: FontWeight.w500,
                    color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                    fontSize: 12,
                  ),
                ),
                const SizedBox(height: 8),
                GridView.count(
                  crossAxisCount: 8,
                  crossAxisSpacing: 4,
                  mainAxisSpacing: 4,
                  shrinkWrap: true,
                  physics: const NeverScrollableScrollPhysics(),
                  children: List.generate(8, (index) {
                    Color bgColor;
                    Color textColor;
                    String label;
                    
                    if (index < 3) {
                      bgColor = isDarkMode ? primaryColor.withOpacity(0.2) : primaryColor.withOpacity(0.1);
                      textColor = isDarkMode ? primaryColor.withOpacity(0.8) : primaryColor;
                      label = 'Water';
                    } else if (index < 5) {
                      bgColor = isDarkMode ? secondaryColor.withOpacity(0.2) : secondaryColor.withOpacity(0.1);
                      textColor = isDarkMode ? secondaryColor.withOpacity(0.8) : secondaryColor;
                      label = 'Light';
                    } else {
                      bgColor = isDarkMode ? Colors.grey[800]! : Colors.grey[100]!;
                      textColor = isDarkMode ? Colors.grey[300]! : Colors.grey[500]!;
                      label = 'Free';
                    }
                    
                    return Column(
                      children: [
                        AspectRatio(
                          aspectRatio: 1,
                          child: Container(
                            decoration: BoxDecoration(
                              color: bgColor,
                              borderRadius: BorderRadius.circular(8),
                            ),
                            alignment: Alignment.center,
                            child: Text(
                              '${index + 1}',
                              style: TextStyle(
                                color: textColor,
                                fontWeight: FontWeight.w500,
                              ),
                            ),
                          ),
                        ),
                        const SizedBox(height: 4),
                        Text(
                          label,
                          style: TextStyle(
                            fontSize: 10,
                            color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                          ),
                        ),
                      ],
                    );
                  }),
                ),
                const SizedBox(height: 16),
                Divider(height: 1, color: isDarkMode ? Colors.grey[800] : Colors.grey[200]),
                const SizedBox(height: 16),
                Text(
                  'Hardware Configuration',
                  style: TextStyle(
                    fontWeight: FontWeight.w500,
                  ),
                ),
                const SizedBox(height: 16),
                Row(
                  children: [
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'GPIO Pin',
                            style: TextStyle(
                              fontWeight: FontWeight.w500,
                              color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                              fontSize: 12,
                            ),
                          ),
                          const SizedBox(height: 8),
                          Container(
                            padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
                            decoration: BoxDecoration(
                              color: isDarkMode ? Colors.grey[800] : Colors.grey[100],
                              borderRadius: BorderRadius.circular(8),
                              border: Border.all(
                                color: isDarkMode ? Colors.grey[700]! : Colors.grey[300]!,
                              ),
                            ),
                            child: DropdownButton<String>(
                              value: 'GPIO 18',
                              isExpanded: true,
                              underline: Container(height: 0),
                              icon: const Icon(Icons.keyboard_arrow_down),
                              items: [
                                'GPIO 18',
                                'GPIO 12',
                                'GPIO 21',
                                'Custom',
                              ].map((String value) {
                                return DropdownMenuItem<String>(
                                  value: value,
                                  child: Text(value),
                                );
                              }).toList(),
                              onChanged: (String? value) {},
                            ),
                          ),
                        ],
                      ),
                    ),
                    const SizedBox(width: 16),
                    Expanded(
                      child: Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Refresh Rate',
                            style: TextStyle(
                              fontWeight: FontWeight.w500,
                              color: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                              fontSize: 12,
                            ),
                          ),
                          const SizedBox(height: 8),
                          Container(
                            padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 4),
                            decoration: BoxDecoration(
                              color: isDarkMode ? Colors.grey[800] : Colors.grey[100],
                              borderRadius: BorderRadius.circular(8),
                              border: Border.all(
                                color: isDarkMode ? Colors.grey[700]! : Colors.grey[300]!,
                              ),
                            ),
                            child: DropdownButton<String>(
                              value: '30 Hz',
                              isExpanded: true,
                              underline: Container(height: 0),
                              icon: const Icon(Icons.keyboard_arrow_down),
                              items: [
                                '30 Hz',
                                '44.1 Hz',
                                '60 Hz',
                                'Custom',
                              ].map((String value) {
                                return DropdownMenuItem<String>(
                                  value: value,
                                  child: Text(value),
                                );
                              }).toList(),
                              onChanged: (String? value) {},
                            ),
                          ),
                        ],
                      ),
                    ),
                  ],
                ),
                const SizedBox(height: 16),
                Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    OutlinedButton.icon(
                      onPressed: () {},
                      icon: const Icon(Icons.refresh),
                      label: const Text('Reset'),
                      style: OutlinedButton.styleFrom(
                        foregroundColor: isDarkMode ? Colors.grey[300] : Colors.grey[700],
                      ),
                    ),
                    ElevatedButton.icon(
                      onPressed: () {},
                      icon: const Icon(Icons.check),
                      label: const Text('Apply'),
                      style: ElevatedButton.styleFrom(
                        backgroundColor: primaryColor,
                        foregroundColor: Colors.white,
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),
        ],
      ),
    );
  }
  
  Widget _buildPlayerControlBar(bool isDarkMode, Color primaryColor) {
    return Container(
      height: 70,
      decoration: BoxDecoration(
        color: isDarkMode ? const Color(0xFF1E1E24) : Colors.white,
        boxShadow: [
          BoxShadow(
            color: Colors.black.withOpacity(0.05),
            blurRadius: 10,
            offset: const Offset(0, -1),
          ),
        ],
        border: Border(
          top: BorderSide(
            color: isDarkMode ? Colors.grey[800]! : Colors.grey[200]!,
            width: 1,
          ),
        ),
      ),
      child: Padding(
        padding: const EdgeInsets.symmetric(horizontal: 16),
        child: Row(
          children: [
            Container(
              width: 40,
              height: 40,
              decoration: BoxDecoration(
                color: isDarkMode ? primaryColor.withOpacity(0.2) : primaryColor.withOpacity(0.1),
                borderRadius: BorderRadius.circular(8),
              ),
              child: Icon(Icons.music_note, color: primaryColor),
            ),
            const SizedBox(width: 12),
            Column(
              crossAxisAlignment: CrossAxisAlignment.start,
              mainAxisAlignment: MainAxisAlignment.center,
              children: [
                const Text('Summer Vibes', style: TextStyle(fontWeight: FontWeight.w500)),
                Text(
                  'Electronic Beats',
                  style: TextStyle(
                    fontSize: 12,
                    color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
                  ),
                ),
              ],
            ),
            const Spacer(),
            IconButton(
              icon: const Icon(Icons.fast_rewind),
              onPressed: () {},
              color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
            ),
            Container(
              width: 40,
              height: 40,
              decoration: BoxDecoration(
                color: primaryColor,
                borderRadius: BorderRadius.circular(20),
              ),
              child: IconButton(
                icon: Icon(_isPlaying ? Icons.pause : Icons.play_arrow),
                color: Colors.white,
                onPressed: _togglePlayPause,
              ),
            ),
            IconButton(
              icon: const Icon(Icons.fast_forward),
              onPressed: () {},
              color: isDarkMode ? Colors.grey[400] : Colors.grey[600],
            ),
          ],
        ),
      ),
    );
  }
}