public class Exercice2 {
    public static class ThreadInfo {
        public volatile long progress = 0;
        public long result = 0;

        public ThreadInfo() {
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
                infos[id].result += (z % 2);
                infos[id].progress++;
            }
        }
    }

    public static ThreadInfo[] infos = new ThreadInfo[4];
    public static long x;
    public static long y = 362436069;
    public static long z = 521288629;

    public static void main(String[] args) throws InterruptedException {
        for (int i = 0; i < infos.length; i++) {
            infos[i] = new ThreadInfo();
        }
        RandomThread[] threads = new RandomThread[]{new RandomThread(0), new RandomThread(1), new RandomThread(2), new RandomThread(3)};
        long startTime = System.currentTimeMillis();
        for (RandomThread t: threads) {
            t.start();
        }
        for (RandomThread t: threads) {
            t.join();
        }
        long elapsedTime = System.currentTimeMillis() - startTime;
        System.out.println("Temps total : " + elapsedTime);
    }
}
