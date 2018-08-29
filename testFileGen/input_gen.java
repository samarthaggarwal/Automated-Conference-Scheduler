import java.io.*;
import java.util.*;

public class input_gen
{
    public static void main(String[] args)
    {
        try
        {
            FileOutputStream fs = new FileOutputStream("input_test.txt", true);
            PrintStream ps = new PrintStream(fs);

            double processingTime = 5.0;
            int k = 10;
            int p = 10;
            int t = 20;
	    int c = 10;
            int n = k * p * t;

            double[][] matrix = new double[n][n];
            for(int i = 0; i < n; i++)
                for(int j = i + 1; j < n; j++)
                {
                    double random = Math.random();
                    matrix[i][j] = random;
                    matrix[j][i] = random;
                }

            ps.println(processingTime);
            ps.println(k);
            ps.println(p);
            ps.println(t);
	    ps.println(c);

            for(int i = 0; i < n; i++)
            {
                for(int j = 0; j < n - 1; j++)
                    ps.print(Math.round(100 * matrix[i][j]) / 100.0 + " ");
                ps.println(Math.round(100 * matrix[i][n - 1]) / 100.0);
            }
        }
        catch(FileNotFoundException e1)
        {
            System.out.println("File Not Found!");
        }
    }
}
