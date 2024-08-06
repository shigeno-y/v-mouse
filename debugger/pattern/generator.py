from pathlib import Path

patterns = {
    -4: 0b1100,
    -3: 0b1101,
    -2: 0b1110,
    -1: 0b1111,
    0:  0b0000,
    1:  0b0001,
    2:  0b0010,
    3:  0b0011,
    4:  0b0100,
}

def convert_patterns(file:Path):
    ret=list()
    ap = ret.append
    lines = file.read_text().splitlines()

    for line in lines:
        line = line.strip()
        curr = tuple([int(x)+4 for x in line.split('\t')])
        dx, dy = curr
        ap((dx<<4) + (dy))

    return ret

def RLE_numpy(sequence):
    '''
    https://qiita.com/isourou/items/a7c32d35a206ec785a6f
    '''

    import numpy as np

    diff_seq = np.diff(sequence) # sequence[i+1] - sequence[i]のアレイ。隣と同じだと0になる。

    # newdata は、前の要素とインデックスが違うときだけTrueになるBoolのアレイ。
    newdata = np.append(True, diff_seq != 0) # 先頭をTrueにして、2番目以降をappendで追加している。
    comp_seq = sequence[newdata]  # sequence から、newdataがTrueの要素だけ抜き出す

    comp_seq_index = np.where(newdata)[0]  # newdataがTrueの要素が、アレイの何番目に来るか取得
    comp_seq_index = np.append(comp_seq_index, len(sequence))  # アレイの終了をつける
    lengths = np.diff(comp_seq_index) # newdataがTrueになっている位置の差がlengthになる

    return comp_seq, lengths

if __name__ == "__main__":
    import argparse
    import numpy as np
    

    parser = argparse.ArgumentParser()
    parser.add_argument("infile", type=Path, metavar="INPUT")
    args = parser.parse_args()

    ps = convert_patterns(args.infile)
    

    t1, t2 = RLE_numpy(np.array(ps))

    print(f'''#ifndef SPIRAL_PATTERN_GENERATED_HPP_INCLUDED
#define SPIRAL_PATTERN_GENERATED_HPP_INCLUDED
// original: {args.infile}
#include <stdint.h>
constexpr uint8_t pattern_length[] = {{''')
    for (p,l) in zip(t1, t2):
        print(p, ",", l, ",")
    print('''};
#endif
''')
