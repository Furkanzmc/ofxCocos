/***********************************************************************

 Copyright (c) 2009, Luke Malcolm, www.lukemalcolm.com

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 ***********************************************************************/

#include "Label.h"
// openFrameworks
#include "ofMain.h"
// utfcpp
#include "utf8.h"
USING_NS_OFXCOCOS

namespace utf8utils
{
void ensureUTF8(std::string &str)
{
    std::string temp;
    utf8::replace_invalid(str.begin(), str.end(), back_inserter(temp));
    str = temp;
}
}

Label::Label()
    : m_Font()
    , m_String("")
    , m_BlankSpaceWord()
    , m_MaxLineWidth(0)
    , m_Alignment(Label::TextAlignment::ALIGN_LEFT)
{

}

Label::~Label()
{

}

Label *Label::create(const string &fontLocation, float fontSize)
{
    Label *lb = new(std::nothrow) Label();
    if (lb && lb->setup(fontLocation, fontSize)) {
        lb->autorelease();
    }
    else {
        SAFE_RELEASE(lb);
    }

    return lb;
}

bool Label::setup(const string &fontLocation, float fontSize)
{
    if (!Node::setup()) {
        return false;
    }

    m_Font.load(fontLocation, fontSize, true, true, true);

    // Set up the blank space word
    m_BlankSpaceWord.rawWord = " ";
    m_BlankSpaceWord.width = m_Font.stringWidth("x");
    m_BlankSpaceWord.height = m_Font.stringHeight("i");
    m_BlankSpaceWord.color = this->getColor();

    this->onColorChanged = std::bind(&Label::updateWordColors, this, std::placeholders::_1);

    return true;
}

void Label::setString(const std::string &inputText)
{
    m_String = inputText;
    loadWords();
    wrapTextForceLines(1);
    updateContentSize();
}

void Label::setMaxLineWidth(float width)
{
    m_MaxLineWidth = width;
    wrapTextWidth(m_MaxLineWidth);
    updateContentSize();
}

float Label::getMaxLineWidth() const
{
    return m_MaxLineWidth;
}

void Label::wrapTextArea(float rWidth, float rHeight)
{
    float tmpScale = 0.0f;
    float maxIterations = getLinedWords();
    float scales[1000];

    if (m_Words.size() > 0) {

        // Check each possible line layout and check it will fit vertically
        for (int iteration = 1; iteration <= maxIterations; iteration++) {

            wrapTextForceLines(iteration);

            tmpScale = rWidth / getWidth();
            if ((tmpScale * getHeight()) < rHeight) {
                scales[iteration] = tmpScale;
            }
            else {
                scales[iteration] = -1;
            }
        }

        // Now see which is biggest
        int maxIndex = 1;
        bool isScaleAvailable = false;

        for (int i = 1; i <= maxIterations; i++) {
            ofLog(OF_LOG_VERBOSE, "Scales %i = %f\n", i, scales[maxIndex]);
            if (scales[i] != -1) isScaleAvailable = true;

            if (scales[i] > scales[maxIndex]) {
                maxIndex = i;
            }
        }

        // When only one line is needed an appropriate on the Y scale can sometimes not be found.  In these occasions scale the size to the Y dimension
        if (isScaleAvailable) {
            this->setScale(scales[maxIndex], scales[maxIndex]);
        }
        else {
            const float scale = (float)rHeight / (float)getHeight();
            this->setScale(scale, scale);
        }

        wrapTextForceLines(maxIndex);

        ofLog(OF_LOG_VERBOSE, "Scaling with %i at scale %f...\n", maxIndex, this->getScale().x);
    }
}

bool Label::wrapTextForceLines(int linesN)
{
    if (m_Words.size() > 0) {
        if (linesN > m_Words.size()) {
            linesN = m_Words.size();
        }

        float lineWidth = m_MaxLineWidth == 0 ? getWidthOfWords() * (1.1f / (float)linesN) : m_MaxLineWidth;
        int curLines = 0;
        bool hasLines = false;

        // Keep increasing the line width until we get the desired number of lines.
        while (!hasLines) {
            curLines = wrapTextWidth(lineWidth);
            if (curLines == linesN) {
                return true;
            }

            if (curLines > linesN) {
                return false;
            }
        }
    }

    return false;
}

void Label::setLineHeight(float lineHeight)
{
    m_Font.setLineHeight(lineHeight);
}

Label::TextAlignment Label::getAlignment() const
{
    return m_Alignment;
}

void Label::setAlignment(const TextAlignment &Alignment)
{
    m_Alignment = Alignment;
}

unsigned int Label::wrapTextWidth(float lineWidth)
{
    if (m_Words.size() > 0) {
        m_Lines.clear();

        float runningWidth = 0.0f;
        bool newLine = true;
        int activeLine = 0;

        LineBlock tmpLine;

        for (int i = 0; i < m_Words.size(); i++) {
            runningWidth += m_Words[i].width;

            if (runningWidth <= lineWidth) {
                newLine = false;
            }
            else {
                newLine = true;
                m_Lines.push_back(tmpLine);
                tmpLine.wordsID.clear();
                runningWidth = 0.0f + m_Words[i].width;;
                activeLine++;
            }

            tmpLine.wordsID.push_back(i);
        }

        // Push in the final line.
        m_Lines.push_back(tmpLine);
        // Trim the leading and trailing spaces.
        trimLineSpaces();

        // Update the content size
        updateContentSize();
    }

    return m_Lines.size();
}

void Label::loadWords()
{
    WordBlock tmpWord;
    istringstream iss(m_String);
    std::vector<std::string> tokens;

    std::copy(istream_iterator<string>(iss), istream_iterator<string>(), std::back_inserter<std::vector<std::string>>(tokens));

    m_Words.clear();
    for (int i = 0; i < tokens.size(); i++) {
        tmpWord.rawWord = tokens.at(i);
        tmpWord.width = m_Font.stringWidth(tmpWord.rawWord);
        tmpWord.height = m_Font.stringHeight(tmpWord.rawWord);
        tmpWord.color = this->getColor();
        m_Words.push_back(tmpWord);

        // Add spaces into the words vector if it is not the last word.
        if (i != tokens.size()) {
            m_Words.push_back(m_BlankSpaceWord);
        }
    }

    for (int i = 0; i < m_Words.size(); i++) {
        ofLog(OF_LOG_VERBOSE, "Loaded word: %i, %s\n", i, m_Words[i].rawWord.c_str());
    }
}

void Label::trimLineSpaces()
{
    if (m_Words.size() > 0) {
        // Now delete all leading or ending spaces on each line
        for (int l = 0; l < m_Lines.size(); l++) {
            // Delete the first word if it is a blank
            if (m_Lines[l].wordsID.size() > 0) {
                if (m_Words[m_Lines[l].wordsID[0]].rawWord == " ")   m_Lines[l].wordsID.erase(m_Lines[l].wordsID.begin());
            }

            // Delete the last word if it is a blank
            if (m_Lines[l].wordsID.size() > 0) {
                if (m_Words[m_Lines[l].wordsID[m_Lines[l].wordsID.size() - 1]].rawWord == " ") m_Lines[l].wordsID.erase(m_Lines[l].wordsID.end() - 1);
            }
        }
    }
}

float Label::getWidthOfWords() const
{
    float widthTotal = 0.0f;
    if (m_Words.size() > 0) {
        for (int i = 0; i < m_Words.size(); i++) {
            widthTotal += m_Words[i].width;
        }

        return widthTotal;
    }

    return 0.0f;
}

int Label::getLinedWords() const
{
    int wordCount = 0;
    if (m_Words.size() > 0) {
        for (unsigned int l = 0; l < m_Lines.size(); l++) {
            wordCount += m_Lines[l].wordsID.size();
        }

        return wordCount;
    }

    return 0;
}

float Label::getLineWidth(const unsigned int &lineIndex) const
{
    if (lineIndex >= m_Lines.size() || m_Words.size() == 0) {
        return 0.f;
    }

    int currentWordID = 0;
    float currX = 0.0f;
    float maxWidth  = 0.0f;
    const LineBlock &targetLine = m_Lines.at(lineIndex);


    for (int wordIndex = 0; wordIndex < targetLine.wordsID.size(); wordIndex++) {
        currentWordID = targetLine.wordsID[wordIndex];

        const WordBlock &currentWord = m_Words[currentWordID];
        currX += currentWord.width;
    }
    maxWidth = std::max(maxWidth, currX);

    return maxWidth;
}

void Label::customDraw() const
{
    if (m_Alignment == TextAlignment::ALIGN_LEFT) {
        drawLeft();
    }
    else if (m_Alignment == TextAlignment::ALIGN_RIGHT) {
        drawRight();
    }
    else if (m_Alignment == TextAlignment::ALIGN_CENTER) {
        drawCenter();
    }
    else if (m_Alignment == TextAlignment::ALIGN_JUSTIFIED) {
        drawJustified();
    }
}

void Label::drawLeft() const
{
    int currentWordID = 0;
    float drawX = 0;
    float drawY = 0;
    float currX = 0;

    if (m_Words.size() > 0) {
        for (unsigned int lineIndex = 0; lineIndex < m_Lines.size(); lineIndex++) {
            const LineBlock &currentLine = m_Lines[lineIndex];

            for (int wordIndex = 0; wordIndex < currentLine.wordsID.size(); wordIndex++) {
                currentWordID = currentLine.wordsID[wordIndex];
                const WordBlock &currentWord = m_Words[currentWordID];

                drawX = currX;
                drawY = (m_Font.getLineHeight() * (lineIndex + 1));

                ofPushStyle();
                {
                    ofSetColor(currentWord.color.r, currentWord.color.g, currentWord.color.b, this->getColor().a);
                    // Because fonts are drawed on bottom-left corner, we move it a little high
                    m_Font.drawStringAsShapes(currentWord.rawWord, drawX, drawY - currentWord.height / 2);
                    currX += currentWord.width;
                }
                ofPopStyle();
            }

            currX = 0;
        }
    }
}

void Label::drawRight() const
{
    //FIXME

    unsigned int currentWordID = 0;
    float drawX = 0;
    float drawY = 0;
    float currX = 0;

    if (m_Words.size() > 0) {
        for (unsigned int lineIndex = 0; lineIndex < m_Lines.size(); lineIndex++) {
            const LineBlock &currentLine = m_Lines[lineIndex];
            const float currentLineWidth = getLineWidth(lineIndex);

            for (auto it = currentLine.wordsID.rbegin(); it != currentLine.wordsID.rend(); it++) {
                const int wordIndex = std::distance(currentLine.wordsID.rbegin(), it);
                currentWordID = currentLine.wordsID[wordIndex];
                const WordBlock &currentWord = m_Words[currentWordID];

                drawX = currentLineWidth - currX - currentWord.width;
                drawY = m_Font.getLineHeight() * (lineIndex + 1);

                ofPushStyle();
                {
                    ofSetColor(currentWord.color.r, currentWord.color.g, currentWord.color.b, this->getColor().a);
                    // Because fonts are drawed on bottom-left corner, we move it a little high
                    m_Font.drawStringAsShapes(currentWord.rawWord.c_str(), drawX, drawY - currentWord.height / 2);
                    currX += currentWord.width;
                }
                ofPopStyle();
            }

            currX = 0;
        }
    }
}

void Label::drawCenter() const
{
    int currentWordID = 0;
    float drawX = 0;
    float drawY = 0;
    float currX = 0;
    float lineWidth = 0;

    if (m_Words.size() > 0) {
        for (int lineIndex = 0; lineIndex < m_Lines.size(); lineIndex++) {
            lineWidth = 0;
            // Get the length of the line.
            for (int wordIndex = 0; wordIndex < m_Lines[lineIndex].wordsID.size(); wordIndex++) {
                currentWordID = m_Lines[lineIndex].wordsID[wordIndex];
                const WordBlock &currentWord = m_Words[currentWordID];
                lineWidth += currentWord.width;
            }

            for (int w = 0; w < m_Lines[lineIndex].wordsID.size(); w++) {
                currentWordID = m_Lines[lineIndex].wordsID[w];
                const WordBlock &currentWord = m_Words[currentWordID];

                drawX = -(lineWidth / 2) + currX;
                drawY = m_Font.getLineHeight() * (lineIndex + 1);

                ofPushStyle();
                {
                    ofSetColor(currentWord.color.r, currentWord.color.g, currentWord.color.b, this->getColor().a);
                    ofPushMatrix();
                    {
                        // Move to central point
                        ofTranslate(m_ContentSize.x / 2, 0, 0.0f);
                        ofScale(this->getScale().x, this->getScale().y, this->getScale().y);
                        // Because fonts are drawed on bottom-left corner, we move it a little high
                        m_Font.drawStringAsShapes(currentWord.rawWord.c_str(), drawX, drawY - currentWord.height / 2);
                        currX += currentWord.width;
                    }
                    ofPopMatrix();
                }
                ofPopStyle();
            }

            currX = 0;
        }
    }
}

void Label::drawJustified() const
{
    const float boxWidth = this->getBoundingBox().width;
    int currentWordID = 0;
    float drawX = 0;
    float drawY = 0;
    int spacesCount = 0;
    float nonSpaceWordWidth = 0;
    float pixelsPerSpace = 0;
    float currX = 0;

    if (m_Words.size() > 0) {
        for (int lineIndex = 0; lineIndex < m_Lines.size(); lineIndex++) {
            const LineBlock &currentLine = m_Lines[lineIndex];

            // Find number of spaces and width of other words
            spacesCount = 0;
            nonSpaceWordWidth = 0;

            for (int wordIndex = 0; wordIndex < currentLine.wordsID.size(); wordIndex++) {
                currentWordID = currentLine.wordsID[wordIndex];
                const WordBlock &currentWord = m_Words[currentWordID];

                if (currentWord.rawWord == " ") {
                    spacesCount++;
                }
                else {
                    nonSpaceWordWidth += currentWord.width;
                }
            }

            pixelsPerSpace = ((boxWidth / this->getScale().x) - nonSpaceWordWidth) / spacesCount;

            for (int wordIndex = 0; wordIndex < currentLine.wordsID.size(); wordIndex++) {
                currentWordID = currentLine.wordsID[wordIndex];
                const WordBlock &currentWord = m_Words[currentWordID];

                drawX = currX;
                drawY = m_Font.getLineHeight() * (lineIndex + 1);

                ofPushStyle();
                {
                    ofSetColor(currentWord.color.r, currentWord.color.g, currentWord.color.b, this->getColor().a);
                    if (currentWord.rawWord != " ") {
                        // Because fonts are drawed on bottom-left corner, we move it a little high
                        m_Font.drawStringAsShapes(currentWord.rawWord.c_str(), drawX, drawY - currentWord.height / 2);
                        currX += currentWord.width;
                    }
                    else {
                        currX += pixelsPerSpace;
                    }
                }
                ofPopStyle();

            }

            currX = 0;
        }
    }
}

float Label::calculateWidth() const
{
    int currentWordID = 0;
    float currX = 0.0f;
    float maxWidth  = 0.0f;

    if (m_Words.size() > 0) {
        for (int lineIndex = 0; lineIndex < m_Lines.size(); lineIndex++) {
            const LineBlock &currentLine = m_Lines[lineIndex];

            for (int wordIndex = 0; wordIndex < currentLine.wordsID.size(); wordIndex++) {
                currentWordID = currentLine.wordsID[wordIndex];

                const WordBlock &currentWord = m_Words[currentWordID];
                currX += currentWord.width;
            }
            maxWidth = std::max(maxWidth, currX);
            currX = 0.0f;
        }

        return maxWidth;
    }

    return 0;
}

float Label::calculateHeight()
{
    if (m_Words.size() > 0) {
        return m_Font.getLineHeight() * m_Lines.size();
    }

    return 0.0f;
}

float Label::getWidth()
{
    return m_ContentSize.x;
}

float Label::getHeight()
{
    return m_ContentSize.y;
}

void Label::updateContentSize()
{
    this->setContentSize(ofVec2f(calculateWidth(), calculateHeight()));
}

void Label::updateWordColors(const ofColor &color)
{
    if (m_Words.size() > 0) {
        for (int i = 0; i < m_Words.size(); i++) {
            m_Words[i].color = color;
        }
    }
}
