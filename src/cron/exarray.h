#pragma once

#include <vector>

template<typename T>
class ExArray : public std::vector<T> {
public:
    inline bool contain(const T&t ) const {
        return std::find(begin(), end(), t) != end();
    }

    /*
     * ��[ib,ie]������Χ�ڲ���t
     * ie=-1��ʱ�򣬷�ΧΪ [ib,size);
     *
     */
    bool contain_within_index(const T&t,int ib, int ie=-1)const {
        if (ib < 0)
            ib = 0;

        size_t c = size();
        if (ib >= c)
            return false;

        if (-1 == ie )
            ie = c - 1;

        for (; ib <= ie; ib++) {
            if (at(ib) == t) {
                return true;
            }
        }
        return false;
    }

    /*
     * ��������Χ[ib,ie]�ڲ��ҵ�һ�����ڵ���t����ֵ
     *
     * ����������������-1 Ϊδ�ҵ�
     *
     */
    int find_first_lgoreq(const T &t, int ib=0,int ie=-1) const {
        if (ib < 0)
            ib = 0;

        int c = size();
        if (ib >= c)
            return -1;

        if (-1 == ie )
            ie = c - 1;

        for (; ib <= ie; ib++) {
            if (at(ib) >= t) {
                return ib;
            }
        }
        return -1;
    }
};

typedef ExArray<int> Ints;

