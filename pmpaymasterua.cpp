#include <billmgr/payment/paymentmodule.h>

using namespace payment;

#define BINARY_NAME "pmpaymasterua"

MODULE(BINARY_NAME);

class Paymasterua : public Module
{
	public:
		Paymasterua(): Module("paymasterua", "1.0")
		{
			feature_map[PAYMENT_FEATURE_REDIRECT] = true;
			feature_map[PAYMENT_FEATURE_PMTUNE] = true;
			param_map[PAYMENT_PARAM_PAYMENT_SCRIPT] = "/mancgi/paymasteruapayment";
			need_profile_info = { "name", "person" };
		}

		void PM_Tune(mgr_xml::Xml& ses) override
		{
			auto slist = ses.GetRoot().AppendChild("slist").SetProp("name", "payment_system");
			std::vector<std::pair<int, string>> methods = {
				{0, "all"},
				{18, "test"},
				{1, "wm"},
				{6, "monexy"},
				{12, "easypay"},
				{15, "nsmep"},
				{17, "wmterminal"},
				{21, "paymastercard"},
				{20, "privat24"},
				{19, "liqpay"},
				{23, "kyivstar"},
				{2, "wmmobile"},
				{29, "bitcoin"}
			};
			for (auto i : methods)
			{
				slist.AppendChild("msg", "pm_" + i.second).SetProp("key", str::Str(i.first));
			}
		}
};

RUN_MODULE(Paymasterua)
