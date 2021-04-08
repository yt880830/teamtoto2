package loginout;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;

import bean.Customer;
import dao.CustomerDAO;
import tool.Action;

public class LoginAction extends Action {
	public String execute(
			HttpServletRequest request, HttpServletResponse response
		) throws Exception {

			HttpSession session=request.getSession();

			String username=request.getParameter("username");
			String password=request.getParameter("password");

			CustomerDAO dao=new CustomerDAO();
			Customer customer=dao.search(username, password);

			if (customer!=null) {
				session.setAttribute("customer", customer);
				return "/grasys/login-out.jsp";
			}

			return "/grasys/login-error.jsp";
		}
}
