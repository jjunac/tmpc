public class Exercice2 {
    public static class ThreadInfo {
        public volatile long process = 0;
        public long result = 0;

        public ThreadInfo() { }
    }

    public static ThreadInfo[] infos = new ThreadInfo[4];
    public static long x;
    public static long y = 362436069;
    public static long z = 521288629;

    public static void main(String[] args) {
        for (int i = 0; i < infos.length; i++) {
            infos[i] = new ThreadInfo();
        }
        long startTime = System.currentTimeMillis();
        for(int i = 0; i < 4; i++){
            Thread thread = new Thread(){
                public void run(){
                    int id = (int) Thread.currentThread().getId()%4;
                    x = System.nanoTime() * (id + 1);
                    for (int i = 0; i < Math.pow(10,8); ++i) {
                        long t;
                        x ^= x << 16;
                        x ^= x >> 5;
                        x ^= x << 1;
                        t = x;
                        x = y;
                        y = z;
                        z = t ^ x ^ y;
                        infos[id].result += (z%2);
                        infos[id].process++;
                    }
                }
            };
            thread.start();
            try {
                thread.join();
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
        long elapsedTime = System.currentTimeMillis() - startTime;
        System.out.println("Temps total : "+elapsedTime);
    }        
}
