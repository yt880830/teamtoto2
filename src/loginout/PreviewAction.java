package loginout;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import tool.Action;

public class PreviewAction extends Action {
	public String execute(
		HttpServletRequest request, HttpServletResponse response
	) throws Exception {
		HttpSession session=request.getSession();

		if (session.getAttribute("customer")==null) {
			return "/grasys/preview-error-login.jsp";
		}

		return "/grasys/itemtouroku.jsp";
	}
}
