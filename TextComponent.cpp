#include "TextComponent.h"

// init static
std::vector<std::string> TextComponent::texts;

TextComponent::TextComponent(std::string text, float size, float r, float g, float b) : size(size), R(r), G(g), B(b) {
    texts.push_back(text);
    textID = texts.size() - 1; // get the last index.
    x = 0;
    y = 0;
}

void TextComponent::clearTexts() {
    texts.clear();
}

void TextComponent::setText(std::string text) {
    texts[textID] = text;
}

std::string TextComponent::getText() {
    return texts[textID];
}
