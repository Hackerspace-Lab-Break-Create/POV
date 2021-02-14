import java.io.IOException;
import java.util.Scanner;

public class Main {

    public static boolean next = true;
    public static Scanner scanner = new Scanner(System.in);
    public static String continuation;

    public static void main(String[] args) throws IOException {

        TerminalController terminalController = new TerminalController();

        while (next) {
            terminalController.init();

            System.out.println("Want to set a different word? Y/N");
            continuation = scanner.nextLine();

            if (continuation.equals("N")) {
                next = false;
                terminalController.closeConnections();
            }
        }
    }
}