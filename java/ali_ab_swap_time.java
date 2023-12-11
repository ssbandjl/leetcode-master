import java.util.*;
public class Main {
    /*
    更改题目   a=[D,B,C,A]; b=[A,B,C,D]
    对 b 做映射  A->0; B->1; C->2; D->3
    那么对应的 a 可根据映射规则更改为 a'=[3,1,2,0]
    之后对 a' 做最长连续上升子数组即可
    */
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);
        int n = sc.nextInt();
        int[] a = new int[n];
        int[] b = new int[n];
        for (int i = 0; i < n; i ++) {
            a[i] = sc.nextInt();
        }
        for (int i = 0; i < n; i ++) {
            b[i] = sc.nextInt();
        }
        // 对 A 里面的数进行重新映射
        Map<Integer, Integer> map = new HashMap<>();
        for (int i = 0; i < n; i ++) {
            map.put(b[i], i);
        }
        // 利用映射规则对 B 进行更新映射
        for (int i = 0; i < n; i ++) {
            a[i] = map.get(a[i]);
        }
        // 对更改之后的 b 做最长连续上升子序列
        int res = 0;
        int cur = 1;
        for (int i = 1; i < n; i ++) {
            if (a[i]>a[i-1]) {
                cur += 1;
                res = Math.max(res, cur);
            } else {
                cur = 1;
            }
        }
        System.out.println(n - res);
    }
}

// 作者：牛客837169005号
// 链接：https://www.nowcoder.com/exam/test/76577735/submission?examPageSource=Company&pid=30440638&testCallback=https%3A%2F%2Fwww.nowcoder.com%2Fexam%2Fcompany%3FcurrentTab%3Drecommand%26jobId%3D100%26selectStatus%3D0%26tagIds%3D134&testclass=%E8%BD%AF%E4%BB%B6%E5%BC%80%E5%8F%91
// 来源：牛客网

