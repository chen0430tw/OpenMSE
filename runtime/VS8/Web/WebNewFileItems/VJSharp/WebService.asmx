<%@ WebService Language= "VJ#" Class="WebService1" %>

import System.Web.*;
import System.Web.Services.*;
import System.Web.Services.Protocols.*;

/** @attribute WebService(Namespace = "http://tempuri.org/") */
/** @attribute WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)*/
public class WebService1 extends System.Web.Services.WebService {
        
    public WebService1()
    {
        //
        // TODO: Add any constructor code required
        //
    }
    // WEB SERVICE EXAMPLE
    // The HelloWorld() example service returns the string Hello World.
    //
	/** @attribute WebMethod() */
	public String HelloWorld() {
		return "Hello World";
	}

}
