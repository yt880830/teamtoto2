package bean;

public class Person implements java.io.Serializable{
	private Customer customer;


	public Customer getCustomer() {
		return customer;
	}


	public void setCustomer(Customer customer) {
		this.customer=customer;
	}


}
