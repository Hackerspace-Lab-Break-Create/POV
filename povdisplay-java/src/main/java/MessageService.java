import org.springframework.stereotype.Service;

import java.util.HashMap;
import java.util.Map;


public class MessageService {

    private String message = "TOFU";

     public String getWord() {

        return message;

    }

    public void setMessage(String message) {
        this.message = message;
    }


}