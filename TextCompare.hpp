////////////////////////////////////////////////////////////////////////////////
//
// File Name:	TextCompare.hpp
// Class Name:  
// Description:	文本相似度匹配
// Author:		罗志彬
// Date:		2019年4月22日
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TEXT_COMPARE_HPP
#define TEXT_COMPARE_HPP
#include <string>
class levenshtein
{
public:
    static  bool compare_char_(char c1, char c2)
    {
        return c1 == c2;
    }

    static size_t ins_(char /*c*/)
    {
        return 1;
    }

    static size_t del_(char /*c*/)
    {
        return 1;
    }

    static size_t sub_(char c1, char c2)
    {
        return compare_char_(c1, c2) ? 0 : 2;
    }

    static size_t compare_(const std::string& ref_s, const std::string& ref_l)
    {
        size_t len_s = ref_s.length();
        size_t len_l = ref_l.length();

        size_t** distance = new size_t*[len_s + 1];
        for(size_t i = 0; i < len_s + 1; ++i)
        {
            distance[i] = new size_t[len_l + 1];
        }

        distance[0][0] = 0;

        for(size_t i = 1; i < len_s + 1; ++i)
        {
            distance[i][0] = distance[i - 1][0] + del_(ref_s.at(i - 1));
        }

        for(size_t i = 1; i < len_l + 1; ++i)
        {
            distance[0][i] = distance[0][i - 1] + ins_(ref_l.at(i - 1));
        }

        for(size_t i = 1; i < len_s + 1; ++i)
        {
            for(size_t j = 1; j < len_l + 1; ++j)
            {
                size_t ins = distance[i][j - 1] + ins_(ref_l.at(j - 1));
                size_t del = distance[i - 1][j] + del_(ref_s.at(i - 1));
                size_t sub = distance[i - 1][j - 1] + sub_(ref_s.at(i - 1), ref_l.at(j - 1));

                distance[i][j] = std::min(std::min(ins, del), sub);
            }
        }

        return distance[len_s][len_l];
    }

    static float compare(const std::string& ref1, const std::string& ref2)
    {
        if(ref1.empty() && ref2.empty())
        {
            return 1;
        }

        size_t distance = 0;
        size_t len = 0;

        if(ref1.length() < ref2.length())
        {
            distance = compare_(ref1, ref2);
            len = ref2.length();
        }
        else
        {
            distance = compare_(ref2, ref1);
            len = ref1.length();
        }

        return distance < len ? 1 - static_cast<float>(distance) / len : 0;
    }
};	//levenshtein;

#endif // NewTec_HPP
