import java.util.Arrays;

import sun.misc.Contended;

public class Exercice2 {

    static final int NB_THREADS = 4;

    public static class ThreadInfo {
        public long progress = 0;
        @Contended
        public long result = 0;

        public ThreadInfo() {
        }

        @Override
        public String toString() {
            return String.format("{progress=%d, result=%d}", progress, result);
        }
    }

    public static class RandomThread extends Thread {
        int id;

        public RandomThread(int id) {
            this.id = id;
        }

        @Override
        public void run() {
            x = System.nanoTime() * (id + 1);
            for (int i1 = 0; i1 < Math.pow(10, 8); ++i1) {
                long t;
                x ^= x << 16;
                x ^= x >> 5;
                x ^= x << 1;
                t = x;
                x = y;
                y = z;
                z = t ^ x ^ y;
                infos[id].result += ((z % 2) + 2) % 2;
                infos[id].progress++;
            }
        }
    }

    public volatile static ThreadInfo[] infos = new ThreadInfo[NB_THREADS];
    public static long x;
    public static long y = 362436069;
    public static long z = 521288629;

    public static void main(String[] args) throws InterruptedException {
        for (int i = 0; i < infos.length; i++) {
            infos[i] = new ThreadInfo();
        }
        RandomThread[] threads = new RandomThread[]{new RandomThread(0), new RandomThread(1), new RandomThread(2), new RandomThread(3)};
        for (RandomThread t: threads) {
            t.start();
        }
        for (RandomThread t: threads) {
            t.join();
        }
        for (int i = 0; i < infos.length; i++) {
            infos[i] = new ThreadInfo();
        }
        System.out.println(Arrays.toString(infos));
    }
}
