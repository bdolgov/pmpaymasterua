#include <payment/paymentcgi.h>
#include <mgr/mgrrpc.h>
#include <mgr/mgrdate.h>
#include <mgr/mgrhash.h>
#include <mgr/mgrstream.h>
#include <sstream>

MODULE("paymasteruapayment");

using namespace payment;

class PaymasteruaPayment : public PaymentCgi
{
	private:
		string FixedLang()
		{
			StringMap m;
			m["ru"] = "ru";
			m["ua"] = "uk";
			m["en"] = "en";
			m["uk"] = "uk";
			auto i = m.find(Lang());
			Warning("lang=%s", Lang().c_str());
			return i != m.end() ? i->second : "en";
		}

	public:
		PaymasteruaPayment(): PaymentCgi("paymasterua", "paymasteruapayment")
		{
		}
		
		bool ValidateAuth() override
		{
			return true;
		}

		void Process() override
		{
			StringMap params;
			params["LMI_MERCHANT_ID"] = Method("merchant_id");
			params["LMI_PAYMENT_AMOUNT"] = Payment("amount");
			params["LMI_PAYMENT_NO"] = Payment("id"); // + "-" + str::hex::Encode(str::Random(5));
			params["elid"] = Payment("id");
			params["LMI_PAYMENT_DESC"] = Payment("description") + " #" + Payment("id");
			params["LMI_SUCCESS_URL"] = GetSuccessPage();
			params["LMI_SUCCESS_METHOD"] = "1";
			params["LMI_FAIL_URL"] = GetFailPage();
			params["LMI_FAIL_METHOD"] = "1";
			string payment_system = Method("payment_system");
			if (payment_system != "0")
			{
				params["LMI_PAYMENT_SYSTEM"] = payment_system;
			}
			params["LMI_PAYER_EMAIL"] = Customer("email");
	
			string sig;
			for (auto i : { "LMI_MERCHANT_ID", "LMI_PAYMENT_NO", "LMI_PAYMENT_AMOUNT" })
			{
				sig += params[i];
			}
			sig += Method("key");

			params["LMI_HASH"] = str::Upper(str::hex::Encode(mgr_hash::sha256(sig)));

			string form_str =
				"<html>\n"
				"	<head>\n"
				"		<meta http-equiv=\"Content-type\" content=\"text/html; charset=utf-8\"/>\n"
				"		<script language=\"JavaScript\">\n"
				"			function DoSubmit() {\n"
				"				document.paymasteruaform.submit();\n"
				"			}\n"
				"		</script>\n"
				"	</head>\n"
				"	<body onload=\"DoSubmit()\">\n"
				"		<form name=\"paymasteruaform\" action=\"https://lmi.paymaster.ua/__LANG__/\" method=\"post\">\n";
			str::inpl::Replace(form_str, "__LANG__", FixedLang());
			for (auto &i : params)
			{
				form_str += "			<input type=\"hidden\" name=\"" + i.first + "\" value=\"" + i.second + "\" />\n";
			}
			form_str += "		</form>\n	</body>\n</html>\n";

			std::cout << form_str;
		}
};

RUN_MODULE(PaymasteruaPayment);
