#include "GeoPage.h"

GeoPage::GeoPage(DataStorage* dataStorage) : _dataStorage(dataStorage) {
}

void GeoPage::render(U8G2_SSD1306_128X64_NONAME_F_HW_I2C* display, uint8_t displayIndex) {
    if (!_dataStorage) return;
    
    SharedData& data = _dataStorage->getData();
    
    // Use larger fonts for better visibility
    display->setFont(u8g2_font_fub30_tf); // Large font for main info
    
    // Get the location strings
    String continent = data.continent;
    String country = data.country;
    String city = data.city;
    
    // Combine all text into one continuous string for distribution
    String allText = continent + " " + country + " " + city;
    
    // Each display can show about 4-5 characters in the large font
    // We'll split the text across displays, 4 chars per display
    const int charsPerDisplay = 4;
    
    if (displayIndex == 0) {
        // First display shows "GEO" label
        display->drawStr(0, 40, "GEO");
    } else if (displayIndex == 7) {
        // Last display clears the screen
        display->clear();
    } else {
        // For displays 1-6, show portions of the location text
        // Adjust index: display 1 starts at 0 of the actual text (after label)
        // Actually, let's use display 1-6 to show up to 6*4 = 24 characters of text
        int textDisplayIndex = displayIndex - 1;
        int textStart = textDisplayIndex * charsPerDisplay;
        int textEnd = min(textStart + charsPerDisplay, (int)allText.length());
        
        if (textStart < (int)allText.length() && allText.length() > 0) {
            String segment = allText.substring(textStart, textEnd);
            display->drawStr(0, 40, segment.c_str());
        } else {
            // No text to show, clear this display? But we can't clear individual displays easily
            // Just show nothing or a placeholder
            // display->clear(); // This would clear the whole display buffer, not good
            // Instead, draw spaces or nothing
            // Actually, if we don't draw anything, the previous content might remain
            // But the display is being redrawn each cycle, so it's okay to leave blank
        }
    }
}
