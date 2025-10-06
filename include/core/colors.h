#pragma once

#define RED Color{.r = 1.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f} // Red (opaque)
#define GREEN                                                                  \
  Color{.r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f}            // Green (opaque)
#define BLUE Color{.r = 0.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f} // Blue (opaque)
#define YELLOW                                                                 \
  Color{.r = 1.0f, .g = 1.0f, .b = 0.0f, .a = 1.0f} // Yellow (opaque)
#define CYAN Color{.r = 0.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f} // Cyan (opaque)
#define MAGENTA                                                                \
  Color{.r = 1.0f, .g = 0.0f, .b = 1.0f, .a = 1.0f} // Magenta (opaque)
#define BLACK                                                                  \
  Color{.r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 1.0f} // Black (opaque)
#define WHITE                                                                  \
  Color{.r = 1.0f, .g = 1.0f, .b = 1.0f, .a = 1.0f}            // White (opaque)
#define GRAY Color{.r = 0.5f, .g = 0.5f, .b = 0.5f, .a = 1.0f} // Gray (opaque)
#define LIGHT_GRAY                                                             \
  Color{.r = 0.75f, .g = 0.75f, .b = 0.75f, .a = 1.0f} // Light Gray (opaque)
#define DARK_GRAY                                                              \
  Color{.r = 0.25f, .g = 0.25f, .b = 0.25f, .a = 1.0f} // Dark Gray (opaque)

#define TRANSLUCENT_RED                                                        \
  Color{.r = 1.0f, .g = 0.0f, .b = 0.0f, .a = 0.5f} // Red with 50% transparency
#define TRANSLUCENT_BLUE                                                       \
  Color{.r = 0.0f, .g = 0.0f, .b = 1.0f, .a = 0.5f}                            \
  // Blue with 50% transparency
#define TRANSLUCENT_GREEN                                                      \
  Color{.r = 0.0f, .g = 1.0f, .b = 0.0f, .a = 0.5f}                            \
  // Green with 50% transparency

#define TRANSPARENT                                                            \
  Color{.r = 0.0f, .g = 0.0f, .b = 0.0f, .a = 0.0f}                            \
  // Fully transparent (invisible)

// Pastels (lighter colors)
#define PASTEL_PINK                                                            \
  Color{.r = 1.0f, .g = 0.7f, .b = 0.8f, .a = 1.0f} // Pastel pink
#define PASTEL_YELLOW                                                          \
  Color{.r = 1.0f, .g = 1.0f, .b = 0.6f, .a = 1.0f} // Pastel yellow
#define PASTEL_BLUE                                                            \
  Color{.r = 0.6f, .g = 0.8f, .b = 1.0f, .a = 1.0f} // Pastel blue

// Darker Colors (shade variants)
#define DARK_RED Color{.r = 0.5f, .g = 0.0f, .b = 0.0f, .a = 1.0f} // Dark red
#define DARK_GREEN                                                             \
  Color{.r = 0.0f, .g = 0.5f, .b = 0.0f, .a = 1.0f} // Dark green
#define DARK_BLUE Color{.r = 0.0f, .g = 0.0f, .b = 0.5f, .a = 1.0f} // Dark blue

// Lighter Colors (tint variants)
#define LIGHT_RED Color{.r = 1.0f, .g = 0.6f, .b = 0.6f, .a = 1.0f} // Light red
#define LIGHT_GREEN                                                            \
  Color{.r = 0.6f, .g = 1.0f, .b = 0.6f, .a = 1.0f} // Light green
#define LIGHT_BLUE                                                             \
  Color{.r = 0.6f, .g = 0.6f, .b = 1.0f, .a = 1.0f} // Light blue
