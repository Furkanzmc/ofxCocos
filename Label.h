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

#ifndef OFXTEXTBLOCK_H
#define OFXTEXTBLOCK_H

#include "Node.h"
#include "ofTrueTypeFont.h"

OFXCOCOS_NS_BEGIN

class Label : public Node
{
public:
    struct WordBlock {
        std::string rawWord = "";
        float width = 0, height = 0;
        ofColor color;
    };

    struct LineBlock {
        std::vector<int> wordsID;
        float width = 0, height = 0;
    };

    enum class TextAlignment {
        ALIGN_LEFT,
        ALIGN_RIGHT,
        ALIGN_JUSTIFIED,
        ALIGN_CENTER
    };

public:
    Label();
    virtual ~Label();

    static Label *create(const std::string &fontLocation, float fontSize);

    void setString(const string &inputText);

    void setMaxLineWidth(float width);
    float getMaxLineWidth() const;

    void wrapTextArea(float rWidth, float rHeight);
    bool wrapTextForceLines(int linesN);

    void setLineHeight(float lineHeight);

    TextAlignment getAlignment() const;
    void setAlignment(const TextAlignment &Alignment);

protected:
    bool setup(const std::string &fontLocation, float fontSize);

private:
    ofTrueTypeFont m_Font;
    std::string m_String;
    WordBlock m_BlankSpaceWord;
    float m_MaxLineWidth;
    TextAlignment m_Alignment;

    std::vector<WordBlock> m_Words;
    std::vector<LineBlock> m_Lines;

private:
    void customDraw() const override;

    /**
     * @brief Returns the number of lines it formed.
     * @param lineWidth
     * @return
     */
    unsigned int wrapTextWidth(float lineWidth);

    void loadWords();
    void trimLineSpaces();

    float getWidthOfWords() const;
    int getLinedWords() const;

    float getLineWidth(const unsigned int &lineIndex) const;

    void drawLeft() const;
    void drawRight() const;
    void drawCenter() const;
    void drawJustified() const;

    float calculateWidth() const;
    float calculateHeight();

    float getWidth();
    float getHeight();
    void updateContentSize();

    void updateWordColors(const ofColor &color);
};

OFXCOCOS_NS_END

#endif // OFXTEXTBLOCK_H
