package string;

/***
 * 题目： 把一个0 1 串（只包含0 1的串）进行排序，你可以交换任意两个位置，问最小交换的次数？
 * @author topwqp
 * 思路： 两个标杆  i代表从左往右   j代表 从右往左  ，i碰到0 和 j碰到1进行交换， 由于是0 -- length 所以是线性的。
 */

public class ZeroOneChangeCount {
    public static void main(String args[]){
        String input = "000010010010001000111001";      
        char[]  array = input.toCharArray();
        int count = 0;
        for(int i = 0,j=array.length-1;i<j; ++i,--j){
            for(;i<j && array[i] =='0';)  ++i; 
            for(;i<j && array[j] =='1';) --j;
            if(i<j) count++;

        }
        System.out.println("最小交换次数为： "+count);
    }


}
