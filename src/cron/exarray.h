#pragma once

#include <vector>

template<typename T>
class ExArray : public std::vector<T> {
	typedef std::vector<T> _base;
public:
    inline bool contain(const T&t ) const {
        return std::find(_base::begin(), _base::end(), t) != _base::end();
    }

    /*
     * ��[ib,ie]������Χ�ڲ���t
     * ie=-1��ʱ�򣬷�ΧΪ [ib,size);
     *
     */
    bool contain_within_index(const T&t, int ib, int ie=-1)const {
		if (ib < 0) {
			ib = 0;
		}

		if (ib >= _base::size()) {
			return false;
		}

		if (-1 >= ie) {
			ie = _base::size() - 1;
		}

        for (; ib <= ie; ib++) {
            if (_base::at(ib) == t) {
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
    int find_first_lgoreq(const T &t, int ib = 0, int ie = -1) const {
		if (ib < 0) {
			ib = 0;
		} 
		
		if (ib >= _base::size()) {
			return -1;
		}

		if (-1 >= ie) {
			ie = _base::size() - 1;
		}

        for (; ib <= ie; ib++) {
            if (_base::at(ib) >= t) {
                return ib;
            }
        }
        return -1;
    }
};

typedef ExArray<int> Ints;

