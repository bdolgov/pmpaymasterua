#include <payment/paymentcgi.h>
#include <mgr/mgrrpc.h>
#include <mgr/mgrdate.h>
#include <mgr/mgrstream.h>
#include <payment/paymentutil.h>
#include <mgr/mgrhash.h>

MODULE("paymasteruaresult");

using namespace payment;

class PaymasteruaResult : public PaymentCgi
{
	private:
		void CheckField(const string& field, const string& value)
		{
			if (Input(field) != value)
			{
				LogError("Wrong value for field %s: got '%s', expected '%s'", field.c_str(), Input(field).c_str(), value.c_str());
				throw mgr_err::Value(field, Input(field));
			}
		}
		
		string NiceParams()
		{
			string ret;
			for (auto i : {"LMI_PAYMENT_NO", "LMI_PAYMENT_AMOUNT", "LMI_PAID_AMOUNT", 
				"LMI_PAYMENT_AMOUNT", "LMI_SYS_PAYMENT_ID", "LMI_SYS_PAYMENT_DATE",
				"LMI_PAYER_IDENTIFIER", "LMI_PAYER_PHONE_NUMBER", "LMI_PAYER_EMAIL", "LMI_MODE"})
			{
				string v = Input(i);
				if (!v.empty())
				{
					ret += string(i) + ":\t" + v + "\n";
				}
			}
			return ret;
		}

		enum PaymentStatus
		{
			psUnknown, psNew, psInPay, psVerify, psPaid, psWaitRefund, psRefund, psFraud, psNewMomental
		};

	public:
		PaymasteruaResult(): PaymentCgi("paymasterua", "paymasteruaresult")
		{
		}

		bool ValidateAuth() override
		{
			return false;
		}

		void Process() override
		{
			CheckField("LMI_MERCHANT_ID", Method("merchant_id"));
			CheckField("LMI_PAYMENT_AMOUNT", Payment("amount"));

			int status = str::Int(Payment("status"));
			if (status != psNew && status != psInPay && status != psVerify && status != psNewMomental)
			{
				throw mgr_err::Value("payment_status", Payment("status"));
			}

			if (Input("LMI_PREREQUEST") == "1")
			{
				SetInPay("");
				std::cout << "YES" << std::endl;
				return;
			}
			
			string sig;
			for (auto i : { "LMI_MERCHANT_ID", "LMI_PAYMENT_NO", "LMI_SYS_PAYMENT_ID",
				"LMI_SYS_PAYMENT_DATE", "LMI_PAYMENT_AMOUNT", "LMI_PAID_AMOUNT",
				"LMI_PAYMENT_SYSTEM", "LMI_MODE"})
			{
				sig += Input(i);
			}
			sig += Method("key");
			Debug("Value to be hashed: %s", sig.c_str());
			CheckField("LMI_HASH", str::Upper(str::hex::Encode(mgr_hash::sha256(sig))));
			
			SetPaid(NiceParams());
		}
};

RUN_MODULE(PaymasteruaResult)
