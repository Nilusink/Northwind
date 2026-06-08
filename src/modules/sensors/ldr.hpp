/**
 * @file ldr.hpp
 * @author Nilusink
 * @brief measure ambient brightness
 * @version 0.1
 * @date 2026-05-30
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#ifdef MODULE_LDR
 namespace ldr
 {
    void setup();
    float measure();
 } // namespace ldr
#endif
