#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

class StringManipulator
{
public:
    static std::string trim(const std::string& str, const char delimiter = '<')
    {
        std::string trimmedStr = str;
        auto first = str.find_first_not_of(delimiter);
        if (std::string::npos != first)
        {
            auto last = str.find_last_not_of(delimiter);
            trimmedStr = str.substr(first, (last - first + 1));
        }
        return trimmedStr;
    }

    static std::vector<std::string> parseTokens(const std::string& str, const char delimiter = ' ')
    {
        std::vector<std::string> strings;
        std::istringstream stream(str);
        std::string token;
        while (std::getline(stream, token, delimiter))
        {
            strings.push_back(token);
        }
        return strings;
    }

    static std::string concatenateSegments(const std::vector<std::string>& segments, const std::string& delimiter = ".")
    {
        std::string result = "";
        for (const auto& segment : segments)
        {
            if (!segment.empty())
            {
                result += segment + delimiter;
            }
        }
        if (!result.empty())
        {
            result.pop_back(); // Remove the last delimiter
        }
        return result;
    }
};

class HrmlBuilder
{
private:
    struct Tag
    {
        Tag(const std::string& name, const std::string& parentTag = "")
        : m_tagName(name)
        , m_parentTag(parentTag)
        {
        }
    
        void addTag(const std::string& parentTag, const std::string& name)
        {
            auto parents = StringManipulator::parseTokens(parentTag, '.');
            std::string lastParent = "";
            if (!parents.empty())
            {
                lastParent = parents[parents.size() - 1];
            }
            if (this->m_tagName == lastParent)
            {
                m_innerTags.push_back(std::make_unique<Tag>(name, parentTag));
            }
            else
            {
                for (const auto& innerTag : m_innerTags)
                {
                    innerTag->addTag(parentTag, name);
                }
            }
        }
    
        Tag* getTag(const std::string& parentTag, const std::string& name)
        {
            Tag* tag = nullptr;
            if (m_tagName == name && m_parentTag == parentTag)
            {
                tag = this;
            }
            for (const auto& innerTag : m_innerTags)
            {
                auto foundTag = innerTag->getTag(parentTag, name);
                if (foundTag != nullptr)
                {
                    tag = foundTag;
                }
            }
            return tag;
        }
    
        void addKeyValuePairs(const std::pair<std::string, std::string>& keyValuePair)
        {
            m_tagKeyValue[keyValuePair.first] = keyValuePair.second;
        }
    
        void addKeyValuePairsToExistingTag(const std::string& tagName, const std::pair<std::string,
            std::string>& keyValuePair)
        {
            if (m_tagName == tagName)
            {
                addKeyValuePairs(keyValuePair);
            }
            else
            {
                for (const auto& innerTag : m_innerTags)
                {
                    innerTag->addKeyValuePairsToExistingTag(tagName, keyValuePair);
                }
            }
        }
    
        void printTagInfo()
        {
            std::cout << "Tag Name: " << m_tagName << std::endl;
            std::cout << "Parent tag: " << m_parentTag << std::endl;
            for (const auto& keyValue : m_tagKeyValue)
            {
                std::cout << "Key: " << keyValue.first << " Value: " << keyValue.second << std::endl;
            }
            for (const auto& innerTag : m_innerTags)
            {
                innerTag->printTagInfo();
            }
        }
    
        std::string m_tagName;
        std::string m_parentTag;
        std::vector<std::unique_ptr<Tag>> m_innerTags;
        std::unordered_map<std::string, std::string> m_tagKeyValue;
    };

    void addKeyValuePairs(const std::vector<std::string>& keyValuePairs)
    {
        // keyValuePairs[0] is the tag name
        for (size_t i = 1; i < keyValuePairs.size(); ++i)
        {
            // Handle tags that look like <tag1 attr1 = "val1" attr2="val2"> and remove quotes from
            // values before adding key-value pairs
            if (keyValuePairs.size() > 2 && keyValuePairs[i + 1] == "="
                && i + 2 < keyValuePairs.size())
            {
                for (auto& rootTag : m_rootTags)
                {
                    rootTag->addKeyValuePairsToExistingTag(keyValuePairs[0],
                        { keyValuePairs[i], StringManipulator::trim(keyValuePairs[i + 2], '"') });
                }
                i += 2;
            }
            else
            {
                std::vector<std::string> keyValue = StringManipulator::parseTokens(keyValuePairs[i],
                    '=');
                if (keyValue.size() == 2)
                {
                    for (auto& rootTag : m_rootTags)
                    {
                        rootTag->addKeyValuePairsToExistingTag(keyValuePairs[0], { keyValue[0],
                            StringManipulator::trim(keyValue[1], '"') });
                    }
                }
            }
        }
    }

    std::vector<std::unique_ptr<Tag>> m_rootTags;
    std::vector<std::string> m_tags;

public:
    HrmlBuilder() : m_tags(1, "")
    {
    }

    void addHrmlLine(const std::string& hrmlLine)
    {
        auto trimmedLine = StringManipulator::trim(hrmlLine, '<');
        trimmedLine = StringManipulator::trim(trimmedLine, ' ');
        trimmedLine = StringManipulator::trim(trimmedLine, '>');
        auto tokens = StringManipulator::parseTokens(trimmedLine);
        std::string parents = StringManipulator::concatenateSegments(m_tags, ".");
        for (size_t i = 1; i < m_tags.size(); ++i)
        {
        }
        // Check for closing tag
        if (!tokens.empty() && tokens[0].front() != '/')
        {
            if (tokens.size() == 1)
            {
                addTag(parents, tokens[0]);
            }
            else if (tokens.size() >= 2)
            {
                addTag(parents, tokens[0]);
                addKeyValuePairs(tokens);
            }
            else
            {
                std::cerr << "Invalid hrml line: \"" << hrmlLine << "\""
                    << std::endl;
            }
            m_tags.push_back(tokens[0]);
        }
        else
        {
            m_tags.pop_back();
        }
    }

    void addTag(const std::string& parentTag, const std::string& tagName)
    {
        if (m_tags.size() == 1)
        {
            m_rootTags.push_back(std::make_unique<Tag>(tagName));
        }
        else
        {
            m_rootTags[m_rootTags.size() - 1]->addTag(parentTag, tagName);
        }
    }

    const std::unordered_map<std::string,
        std::string> getKeyValuePairs(const std::string& parentTag,
        const std::string& tagName)const
    {
        std::unordered_map<std::string, std::string> keyValuePairs;
        for (auto& rootTag : m_rootTags)
        {
            auto tag = rootTag->getTag(parentTag, tagName);
            if (tag)
            {
                keyValuePairs = tag->m_tagKeyValue;
            }
        }
        return keyValuePairs;
    }
};

class HrmlDecorator
{
public:
    HrmlDecorator(const HrmlBuilder& hrmlBuilder) : m_hrmlBuilder(hrmlBuilder)
    {
    }
    virtual ~HrmlDecorator(){};
    virtual void showHrmlResponse()const = 0;
    virtual void generateResponseFromQuery(const std::string& query) = 0;
protected:
    const HrmlBuilder& m_hrmlBuilder;
};

class HrmlConsoleDecorator: public HrmlDecorator
{
public:
    HrmlConsoleDecorator(const HrmlBuilder& hrmlBuilder) : HrmlDecorator(hrmlBuilder)
    {
    }

    virtual void showHrmlResponse()const override
    {
        std::cout << m_response << std::endl;
    }

    // Handle tagname.tagname2.tagname3~key queries
    virtual void generateResponseFromQuery(const std::string& query) override
    {
        m_response = "Not Found!";
        auto tokens = StringManipulator::parseTokens(query, '.');
        // Concatenate segments from tokens minus the last one
        std::vector<std::string> parentTokens = std::vector<std::string>(tokens.begin(), tokens.end() - 1);
        std::string parents = StringManipulator::concatenateSegments(parentTokens, ".");
        for (size_t i = 0; i < tokens.size(); ++i)
        {
            auto tagKey = StringManipulator::parseTokens(tokens[i], '~');
            if (i == tokens.size() - 1 && tagKey.size() > 1)
            {
                auto keyValues = m_hrmlBuilder.getKeyValuePairs(parents, tagKey[0]);
                auto keyValue = keyValues.find(tagKey[1]);
                if (keyValue != keyValues.end())
                {
                    m_response = keyValue->second;
                }
            }
        }
    }
private:
    std::string m_response;
};

int main() {
    size_t numTags = 0;
    size_t numQueries = 0;
    HrmlBuilder hrmlBuilder;
    std::cin >> numTags >> numQueries;
    if (numTags % 2 != 0)
    {
        // Opening tags need to be paired with closing tags
        return 1;
    }
    for (size_t i = 0; i < numTags; ++i)
    {
        std::string command;
        // Read cin into the command until reaching the '>' character
        while (command.find('>') == std::string::npos)
        {
            std::string temp;
            std::cin >> temp;
            command += temp + " ";
        }
        hrmlBuilder.addHrmlLine(command);
    }
    for (size_t i = 0; i < numQueries; ++i)
    {
        std::string query;
        std::cin >> query;
        HrmlConsoleDecorator hrmlConsoleDecorator(hrmlBuilder);
        hrmlConsoleDecorator.generateResponseFromQuery(query);
        hrmlConsoleDecorator.showHrmlResponse();
    }
    return 0;
}
