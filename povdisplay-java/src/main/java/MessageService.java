import java.util.HashMap;
import java.util.Map;

public class MessageService {

    private Map<String, Integer[]> lettersMap = new HashMap<>();

    private String message = "TOFUU";

     public String getWord() {

        return message;

    }

    public void setMessage(String message) {
        this.message = message;
    }


}