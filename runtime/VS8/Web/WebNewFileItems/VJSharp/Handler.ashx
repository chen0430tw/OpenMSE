<%@ WebHandler Language="VJ#" Class="GenericHandler1" %>

import System.*;
import System.Web.*;

public class GenericHandler1 implements IHttpHandler {
    
    public void ProcessRequest (HttpContext context) {
        context.get_Response().set_ContentType("text/plain");
        context.get_Response().Write("Hello World");
    }
 
    public boolean get_IsReusable() {
        return false;
    }

}