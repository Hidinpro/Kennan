/* soapServer.c
   Generated by gSOAP 2.8.131 for onvif.h

gSOAP XML Web services tools
Copyright (C) 2000-2023, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#if defined(__BORLANDC__)
#pragma option push -w-8060
#pragma option push -w-8004
#endif
#include "soapH.h"

SOAP_SOURCE_STAMP("@(#) soapServer.c ver 2.8.131 2023-12-05 09:21:32 GMT")
SOAP_FMAC5 int SOAP_FMAC6 soap_serve(struct soap *soap)
{
#ifndef WITH_FASTCGI
	soap->keep_alive = soap->max_keep_alive + 1;
#endif
	do
	{
#ifndef WITH_FASTCGI
		if (soap->keep_alive > 0 && soap->max_keep_alive > 0)
			soap->keep_alive--;
#endif
		if (soap_begin_serve(soap))
		{	if (soap->error >= SOAP_STOP)
				continue;
			return soap->error;
		}
		if ((soap_serve_request(soap) || (soap->fserveloop && soap->fserveloop(soap))) && soap->error && soap->error < SOAP_STOP)
		{
#ifdef WITH_FASTCGI
			soap_send_fault(soap);
#else
			return soap_send_fault(soap);
#endif
		}
#ifdef WITH_FASTCGI
		soap_destroy(soap);
		soap_end(soap);
	} while (1);
#else
	} while (soap->keep_alive);
#endif
	return SOAP_OK;
}

#ifndef WITH_NOSERVEREQUEST
SOAP_FMAC5 int SOAP_FMAC6 soap_serve_request(struct soap *soap)
{
	(void)soap_peek_element(soap);
	if (!soap_match_tag(soap, soap->tag, "SOAP-ENV:Fault"))
		return soap_serve_SOAP_ENV__Fault(soap);
	if (!soap_match_tag(soap, soap->tag, "wsdd:Hello"))
		return soap_serve___wsdd__Hello(soap);
	if (!soap_match_tag(soap, soap->tag, "wsdd:Bye"))
		return soap_serve___wsdd__Bye(soap);
	if (!soap_match_tag(soap, soap->tag, "wsdd:Probe"))
		return soap_serve___wsdd__Probe(soap);
	if (!soap_match_tag(soap, soap->tag, "wsdd:ProbeMatches"))
		return soap_serve___wsdd__ProbeMatches(soap);
	if (!soap_match_tag(soap, soap->tag, "wsdd:Resolve"))
		return soap_serve___wsdd__Resolve(soap);
	if (!soap_match_tag(soap, soap->tag, "wsdd:ResolveMatches"))
		return soap_serve___wsdd__ResolveMatches(soap);
	if (!soap_match_tag(soap, soap->tag, "SOAP-ENV:Fault-alex"))
		return soap_serve_SOAP_ENV__Fault_alex(soap);
	if (!soap_match_tag(soap, soap->tag, "tdn:Hello"))
		return soap_serve___tdn__Hello(soap);
	if (!soap_match_tag(soap, soap->tag, "tdn:Bye"))
		return soap_serve___tdn__Bye(soap);
	if (!soap_match_tag(soap, soap->tag, "tdn:Probe"))
		return soap_serve___tdn__Probe(soap);
	if (!soap_match_tag(soap, soap->tag, "tds:GetServices"))
		return soap_serve___tds__GetServices(soap);
	if (!soap_match_tag(soap, soap->tag, "tds:GetServiceCapabilities"))
		return soap_serve___tds__GetServiceCapabilities(soap);
	if (!soap_match_tag(soap, soap->tag, "tds:GetDeviceInformation"))
		return soap_serve___tds__GetDeviceInformation(soap);
	if (!soap_match_tag(soap, soap->tag, "tds:SystemReboot"))
		return soap_serve___tds__SystemReboot(soap);
	if (!soap_match_tag(soap, soap->tag, "tds:GetCapabilities"))
		return soap_serve___tds__GetCapabilities(soap);
	if (!soap_match_tag(soap, soap->tag, "trt:GetServiceCapabilities"))
		return soap_serve___trt__GetServiceCapabilities(soap);
	if (!soap_match_tag(soap, soap->tag, "trt:CreateProfile"))
		return soap_serve___trt__CreateProfile(soap);
	if (!soap_match_tag(soap, soap->tag, "trt:GetProfile"))
		return soap_serve___trt__GetProfile(soap);
	if (!soap_match_tag(soap, soap->tag, "trt:GetProfiles"))
		return soap_serve___trt__GetProfiles(soap);
	if (!soap_match_tag(soap, soap->tag, "trt:GetSnapshotUri"))
		return soap_serve___trt__GetSnapshotUri(soap);
	return soap->error = SOAP_NO_METHOD;
}
#endif

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_SOAP_ENV__Fault(struct soap *soap)
{	struct SOAP_ENV__Fault soap_tmp_SOAP_ENV__Fault;
	soap_default_SOAP_ENV__Fault(soap, &soap_tmp_SOAP_ENV__Fault);
	if (!soap_get_SOAP_ENV__Fault(soap, &soap_tmp_SOAP_ENV__Fault, "SOAP-ENV:Fault", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = SOAP_ENV__Fault(soap, soap_tmp_SOAP_ENV__Fault.faultcode, soap_tmp_SOAP_ENV__Fault.faultstring, soap_tmp_SOAP_ENV__Fault.faultactor, soap_tmp_SOAP_ENV__Fault.detail, soap_tmp_SOAP_ENV__Fault.SOAP_ENV__Code, soap_tmp_SOAP_ENV__Fault.SOAP_ENV__Reason, soap_tmp_SOAP_ENV__Fault.SOAP_ENV__Node, soap_tmp_SOAP_ENV__Fault.SOAP_ENV__Role, soap_tmp_SOAP_ENV__Fault.SOAP_ENV__Detail);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__Hello(struct soap *soap)
{	struct __wsdd__Hello soap_tmp___wsdd__Hello;
	soap_default___wsdd__Hello(soap, &soap_tmp___wsdd__Hello);
	if (!soap_get___wsdd__Hello(soap, &soap_tmp___wsdd__Hello, "-wsdd:Hello", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __wsdd__Hello(soap, soap_tmp___wsdd__Hello.wsdd__Hello);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__Bye(struct soap *soap)
{	struct __wsdd__Bye soap_tmp___wsdd__Bye;
	soap_default___wsdd__Bye(soap, &soap_tmp___wsdd__Bye);
	if (!soap_get___wsdd__Bye(soap, &soap_tmp___wsdd__Bye, "-wsdd:Bye", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __wsdd__Bye(soap, soap_tmp___wsdd__Bye.wsdd__Bye);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__Probe(struct soap *soap)
{	struct __wsdd__Probe soap_tmp___wsdd__Probe;
	soap_default___wsdd__Probe(soap, &soap_tmp___wsdd__Probe);
	if (!soap_get___wsdd__Probe(soap, &soap_tmp___wsdd__Probe, "-wsdd:Probe", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __wsdd__Probe(soap, soap_tmp___wsdd__Probe.wsdd__Probe);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__ProbeMatches(struct soap *soap)
{	struct __wsdd__ProbeMatches soap_tmp___wsdd__ProbeMatches;
	soap_default___wsdd__ProbeMatches(soap, &soap_tmp___wsdd__ProbeMatches);
	if (!soap_get___wsdd__ProbeMatches(soap, &soap_tmp___wsdd__ProbeMatches, "-wsdd:ProbeMatches", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __wsdd__ProbeMatches(soap, soap_tmp___wsdd__ProbeMatches.wsdd__ProbeMatches);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__Resolve(struct soap *soap)
{	struct __wsdd__Resolve soap_tmp___wsdd__Resolve;
	soap_default___wsdd__Resolve(soap, &soap_tmp___wsdd__Resolve);
	if (!soap_get___wsdd__Resolve(soap, &soap_tmp___wsdd__Resolve, "-wsdd:Resolve", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __wsdd__Resolve(soap, soap_tmp___wsdd__Resolve.wsdd__Resolve);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___wsdd__ResolveMatches(struct soap *soap)
{	struct __wsdd__ResolveMatches soap_tmp___wsdd__ResolveMatches;
	soap_default___wsdd__ResolveMatches(soap, &soap_tmp___wsdd__ResolveMatches);
	if (!soap_get___wsdd__ResolveMatches(soap, &soap_tmp___wsdd__ResolveMatches, "-wsdd:ResolveMatches", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __wsdd__ResolveMatches(soap, soap_tmp___wsdd__ResolveMatches.wsdd__ResolveMatches);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve_SOAP_ENV__Fault_alex(struct soap *soap)
{	struct SOAP_ENV__Fault_alex soap_tmp_SOAP_ENV__Fault_alex;
	soap_default_SOAP_ENV__Fault_alex(soap, &soap_tmp_SOAP_ENV__Fault_alex);
	if (!soap_get_SOAP_ENV__Fault_alex(soap, &soap_tmp_SOAP_ENV__Fault_alex, "SOAP-ENV:Fault-alex", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = SOAP_ENV__Fault_alex(soap, soap_tmp_SOAP_ENV__Fault_alex.faultcode, soap_tmp_SOAP_ENV__Fault_alex.faultstring, soap_tmp_SOAP_ENV__Fault_alex.faultactor, soap_tmp_SOAP_ENV__Fault_alex.detail, soap_tmp_SOAP_ENV__Fault_alex.SOAP_ENV__Code, soap_tmp_SOAP_ENV__Fault_alex.SOAP_ENV__Reason, soap_tmp_SOAP_ENV__Fault_alex.SOAP_ENV__Node, soap_tmp_SOAP_ENV__Fault_alex.SOAP_ENV__Role, soap_tmp_SOAP_ENV__Fault_alex.SOAP_ENV__Detail);
	if (soap->error)
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tdn__Hello(struct soap *soap)
{	struct __tdn__Hello soap_tmp___tdn__Hello;
	struct wsdd__ResolveType tdn__HelloResponse;
	soap_default_wsdd__ResolveType(soap, &tdn__HelloResponse);
	soap_default___tdn__Hello(soap, &soap_tmp___tdn__Hello);
	if (!soap_get___tdn__Hello(soap, &soap_tmp___tdn__Hello, "-tdn:Hello", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __tdn__Hello(soap, soap_tmp___tdn__Hello.tdn__Hello, &tdn__HelloResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize_wsdd__ResolveType(soap, &tdn__HelloResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_wsdd__ResolveType(soap, &tdn__HelloResponse, "tdn:HelloResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_wsdd__ResolveType(soap, &tdn__HelloResponse, "tdn:HelloResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tdn__Bye(struct soap *soap)
{	struct __tdn__Bye soap_tmp___tdn__Bye;
	struct wsdd__ResolveType tdn__ByeResponse;
	soap_default_wsdd__ResolveType(soap, &tdn__ByeResponse);
	soap_default___tdn__Bye(soap, &soap_tmp___tdn__Bye);
	if (!soap_get___tdn__Bye(soap, &soap_tmp___tdn__Bye, "-tdn:Bye", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __tdn__Bye(soap, soap_tmp___tdn__Bye.tdn__Bye, &tdn__ByeResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize_wsdd__ResolveType(soap, &tdn__ByeResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_wsdd__ResolveType(soap, &tdn__ByeResponse, "tdn:ByeResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_wsdd__ResolveType(soap, &tdn__ByeResponse, "tdn:ByeResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tdn__Probe(struct soap *soap)
{	struct __tdn__Probe soap_tmp___tdn__Probe;
	struct wsdd__ProbeMatchesType tdn__ProbeResponse;
	soap_default_wsdd__ProbeMatchesType(soap, &tdn__ProbeResponse);
	soap_default___tdn__Probe(soap, &soap_tmp___tdn__Probe);
	if (!soap_get___tdn__Probe(soap, &soap_tmp___tdn__Probe, "-tdn:Probe", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __tdn__Probe(soap, soap_tmp___tdn__Probe.tdn__Probe, &tdn__ProbeResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize_wsdd__ProbeMatchesType(soap, &tdn__ProbeResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put_wsdd__ProbeMatchesType(soap, &tdn__ProbeResponse, "tdn:ProbeResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put_wsdd__ProbeMatchesType(soap, &tdn__ProbeResponse, "tdn:ProbeResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetServices(struct soap *soap)
{	struct __tds__GetServices soap_tmp___tds__GetServices;
	struct _tds__GetServicesResponse tds__GetServicesResponse;
	soap_default__tds__GetServicesResponse(soap, &tds__GetServicesResponse);
	soap_default___tds__GetServices(soap, &soap_tmp___tds__GetServices);
	if (!soap_get___tds__GetServices(soap, &soap_tmp___tds__GetServices, "-tds:GetServices", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __tds__GetServices(soap, soap_tmp___tds__GetServices.tds__GetServices, &tds__GetServicesResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize__tds__GetServicesResponse(soap, &tds__GetServicesResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put__tds__GetServicesResponse(soap, &tds__GetServicesResponse, "tds:GetServicesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put__tds__GetServicesResponse(soap, &tds__GetServicesResponse, "tds:GetServicesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetServiceCapabilities(struct soap *soap)
{	struct __tds__GetServiceCapabilities soap_tmp___tds__GetServiceCapabilities;
	struct _tds__GetServiceCapabilitiesResponse tds__GetServiceCapabilitiesResponse;
	soap_default__tds__GetServiceCapabilitiesResponse(soap, &tds__GetServiceCapabilitiesResponse);
	soap_default___tds__GetServiceCapabilities(soap, &soap_tmp___tds__GetServiceCapabilities);
	if (!soap_get___tds__GetServiceCapabilities(soap, &soap_tmp___tds__GetServiceCapabilities, "-tds:GetServiceCapabilities", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __tds__GetServiceCapabilities(soap, soap_tmp___tds__GetServiceCapabilities.tds__GetServiceCapabilities, &tds__GetServiceCapabilitiesResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize__tds__GetServiceCapabilitiesResponse(soap, &tds__GetServiceCapabilitiesResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put__tds__GetServiceCapabilitiesResponse(soap, &tds__GetServiceCapabilitiesResponse, "tds:GetServiceCapabilitiesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put__tds__GetServiceCapabilitiesResponse(soap, &tds__GetServiceCapabilitiesResponse, "tds:GetServiceCapabilitiesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetDeviceInformation(struct soap *soap)
{	struct __tds__GetDeviceInformation soap_tmp___tds__GetDeviceInformation;
	struct _tds__GetDeviceInformationResponse tds__GetDeviceInformationResponse;
	soap_default__tds__GetDeviceInformationResponse(soap, &tds__GetDeviceInformationResponse);
	soap_default___tds__GetDeviceInformation(soap, &soap_tmp___tds__GetDeviceInformation);
	if (!soap_get___tds__GetDeviceInformation(soap, &soap_tmp___tds__GetDeviceInformation, "-tds:GetDeviceInformation", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __tds__GetDeviceInformation(soap, soap_tmp___tds__GetDeviceInformation.tds__GetDeviceInformation, &tds__GetDeviceInformationResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize__tds__GetDeviceInformationResponse(soap, &tds__GetDeviceInformationResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put__tds__GetDeviceInformationResponse(soap, &tds__GetDeviceInformationResponse, "tds:GetDeviceInformationResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put__tds__GetDeviceInformationResponse(soap, &tds__GetDeviceInformationResponse, "tds:GetDeviceInformationResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__SystemReboot(struct soap *soap)
{	struct __tds__SystemReboot soap_tmp___tds__SystemReboot;
	struct _tds__SystemRebootResponse tds__SystemRebootResponse;
	soap_default__tds__SystemRebootResponse(soap, &tds__SystemRebootResponse);
	soap_default___tds__SystemReboot(soap, &soap_tmp___tds__SystemReboot);
	if (!soap_get___tds__SystemReboot(soap, &soap_tmp___tds__SystemReboot, "-tds:SystemReboot", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __tds__SystemReboot(soap, soap_tmp___tds__SystemReboot.tds__SystemReboot, &tds__SystemRebootResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize__tds__SystemRebootResponse(soap, &tds__SystemRebootResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put__tds__SystemRebootResponse(soap, &tds__SystemRebootResponse, "tds:SystemRebootResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put__tds__SystemRebootResponse(soap, &tds__SystemRebootResponse, "tds:SystemRebootResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___tds__GetCapabilities(struct soap *soap)
{	struct __tds__GetCapabilities soap_tmp___tds__GetCapabilities;
	struct _tds__GetCapabilitiesResponse tds__GetCapabilitiesResponse;
	soap_default__tds__GetCapabilitiesResponse(soap, &tds__GetCapabilitiesResponse);
	soap_default___tds__GetCapabilities(soap, &soap_tmp___tds__GetCapabilities);
	if (!soap_get___tds__GetCapabilities(soap, &soap_tmp___tds__GetCapabilities, "-tds:GetCapabilities", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __tds__GetCapabilities(soap, soap_tmp___tds__GetCapabilities.tds__GetCapabilities, &tds__GetCapabilitiesResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize__tds__GetCapabilitiesResponse(soap, &tds__GetCapabilitiesResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put__tds__GetCapabilitiesResponse(soap, &tds__GetCapabilitiesResponse, "tds:GetCapabilitiesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put__tds__GetCapabilitiesResponse(soap, &tds__GetCapabilitiesResponse, "tds:GetCapabilitiesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetServiceCapabilities(struct soap *soap)
{	struct __trt__GetServiceCapabilities soap_tmp___trt__GetServiceCapabilities;
	struct _trt__GetServiceCapabilitiesResponse trt__GetServiceCapabilitiesResponse;
	soap_default__trt__GetServiceCapabilitiesResponse(soap, &trt__GetServiceCapabilitiesResponse);
	soap_default___trt__GetServiceCapabilities(soap, &soap_tmp___trt__GetServiceCapabilities);
	if (!soap_get___trt__GetServiceCapabilities(soap, &soap_tmp___trt__GetServiceCapabilities, "-trt:GetServiceCapabilities", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __trt__GetServiceCapabilities(soap, soap_tmp___trt__GetServiceCapabilities.trt__GetServiceCapabilities, &trt__GetServiceCapabilitiesResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize__trt__GetServiceCapabilitiesResponse(soap, &trt__GetServiceCapabilitiesResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put__trt__GetServiceCapabilitiesResponse(soap, &trt__GetServiceCapabilitiesResponse, "trt:GetServiceCapabilitiesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put__trt__GetServiceCapabilitiesResponse(soap, &trt__GetServiceCapabilitiesResponse, "trt:GetServiceCapabilitiesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__CreateProfile(struct soap *soap)
{	struct __trt__CreateProfile soap_tmp___trt__CreateProfile;
	struct _trt__CreateProfileResponse trt__CreateProfileResponse;
	soap_default__trt__CreateProfileResponse(soap, &trt__CreateProfileResponse);
	soap_default___trt__CreateProfile(soap, &soap_tmp___trt__CreateProfile);
	if (!soap_get___trt__CreateProfile(soap, &soap_tmp___trt__CreateProfile, "-trt:CreateProfile", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __trt__CreateProfile(soap, soap_tmp___trt__CreateProfile.trt__CreateProfile, &trt__CreateProfileResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize__trt__CreateProfileResponse(soap, &trt__CreateProfileResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put__trt__CreateProfileResponse(soap, &trt__CreateProfileResponse, "trt:CreateProfileResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put__trt__CreateProfileResponse(soap, &trt__CreateProfileResponse, "trt:CreateProfileResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetProfile(struct soap *soap)
{	struct __trt__GetProfile soap_tmp___trt__GetProfile;
	struct _trt__GetProfileResponse trt__GetProfileResponse;
	soap_default__trt__GetProfileResponse(soap, &trt__GetProfileResponse);
	soap_default___trt__GetProfile(soap, &soap_tmp___trt__GetProfile);
	if (!soap_get___trt__GetProfile(soap, &soap_tmp___trt__GetProfile, "-trt:GetProfile", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __trt__GetProfile(soap, soap_tmp___trt__GetProfile.trt__GetProfile, &trt__GetProfileResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize__trt__GetProfileResponse(soap, &trt__GetProfileResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put__trt__GetProfileResponse(soap, &trt__GetProfileResponse, "trt:GetProfileResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put__trt__GetProfileResponse(soap, &trt__GetProfileResponse, "trt:GetProfileResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetProfiles(struct soap *soap)
{	struct __trt__GetProfiles soap_tmp___trt__GetProfiles;
	struct _trt__GetProfilesResponse trt__GetProfilesResponse;
	soap_default__trt__GetProfilesResponse(soap, &trt__GetProfilesResponse);
	soap_default___trt__GetProfiles(soap, &soap_tmp___trt__GetProfiles);
	if (!soap_get___trt__GetProfiles(soap, &soap_tmp___trt__GetProfiles, "-trt:GetProfiles", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __trt__GetProfiles(soap, soap_tmp___trt__GetProfiles.trt__GetProfiles, &trt__GetProfilesResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize__trt__GetProfilesResponse(soap, &trt__GetProfilesResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put__trt__GetProfilesResponse(soap, &trt__GetProfilesResponse, "trt:GetProfilesResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put__trt__GetProfilesResponse(soap, &trt__GetProfilesResponse, "trt:GetProfilesResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_serve___trt__GetSnapshotUri(struct soap *soap)
{	struct __trt__GetSnapshotUri soap_tmp___trt__GetSnapshotUri;
	struct _trt__GetSnapshotUriResponse trt__GetSnapshotUriResponse;
	soap_default__trt__GetSnapshotUriResponse(soap, &trt__GetSnapshotUriResponse);
	soap_default___trt__GetSnapshotUri(soap, &soap_tmp___trt__GetSnapshotUri);
	if (!soap_get___trt__GetSnapshotUri(soap, &soap_tmp___trt__GetSnapshotUri, "-trt:GetSnapshotUri", NULL))
		return soap->error;
	if (soap_body_end_in(soap)
	 || soap_envelope_end_in(soap)
	 || soap_end_recv(soap))
		return soap->error;
	soap->error = __trt__GetSnapshotUri(soap, soap_tmp___trt__GetSnapshotUri.trt__GetSnapshotUri, &trt__GetSnapshotUriResponse);
	if (soap->error)
		return soap->error;
	soap->encodingStyle = NULL; /* use SOAP literal style */
	soap_serializeheader(soap);
	soap_serialize__trt__GetSnapshotUriResponse(soap, &trt__GetSnapshotUriResponse);
	if (soap_begin_count(soap))
		return soap->error;
	if ((soap->mode & SOAP_IO_LENGTH))
	{	if (soap_envelope_begin_out(soap)
		 || soap_putheader(soap)
		 || soap_body_begin_out(soap)
		 || soap_put__trt__GetSnapshotUriResponse(soap, &trt__GetSnapshotUriResponse, "trt:GetSnapshotUriResponse", "")
		 || soap_body_end_out(soap)
		 || soap_envelope_end_out(soap))
			 return soap->error;
	};
	if (soap_end_count(soap)
	 || soap_response(soap, SOAP_OK)
	 || soap_envelope_begin_out(soap)
	 || soap_putheader(soap)
	 || soap_body_begin_out(soap)
	 || soap_put__trt__GetSnapshotUriResponse(soap, &trt__GetSnapshotUriResponse, "trt:GetSnapshotUriResponse", "")
	 || soap_body_end_out(soap)
	 || soap_envelope_end_out(soap)
	 || soap_end_send(soap))
		return soap->error;
	return soap_closesock(soap);
}

#if defined(__BORLANDC__)
#pragma option pop
#pragma option pop
#endif

/* End of soapServer.c */
