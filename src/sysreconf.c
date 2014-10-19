#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dtsapp.h>

enum itype {
	IFACE,
	VLAN,
	ALIAS
};

struct intface {
	char ipaddr[16];
	char snbits[4];
	char dhcps[16];
	char dhcpe[16];
	char bwin[16];
	char bwout[16];
	char macaddr[18];
	char gateway[16];
	char intname[64];
	char iface[16];
	int id;
	enum itype type;
	int idx;
	struct intface *next;
};

struct intface *iflst = NULL;
struct xml_doc *xmldata;

struct intface *new_iface(enum itype type, int idx) {
	struct intface *iface;

	if (!(iface = malloc(sizeof(struct intface)))) {
		return NULL;
	}
	strcpy(iface->ipaddr, "0.0.0.0");
	strcpy(iface->snbits, "32");
	strcpy(iface->dhcps, "-");
	strcpy(iface->dhcpe, "-");
	strcpy(iface->bwin, "");
	strcpy(iface->bwout, "");
	strcpy(iface->macaddr, "00:00:00:00:00:00");
	strcpy(iface->gateway, "");
	strcpy(iface->intname, "");
	strcpy(iface->iface, "");
	iface->id = 0;
	iface->type = type;
	iface->idx = idx;
	iface->next = NULL;

	return iface;
}

struct intface *get_iface(enum itype type, int idx) {
	struct intface *last, *tmp = iflst;

	if (!iflst) {
		iflst = new_iface(type, idx);
		return iflst;
	}

	for(;tmp;tmp = tmp->next) {
		if ((tmp->idx == idx) && (tmp->type == type)) {
			break;
		}
		last = tmp;
	}
	if (!tmp) {
		last->next = new_iface(type, idx);
		return last->next;
	}

	return tmp;
}

void handle_iface_opts(char *name, int idx, char *val) {
	struct intface *iface;

	if (!strcmp("IP_ADDR", name)) {
		iface = get_iface(IFACE, idx);
		strncpy(iface->ipaddr, val, sizeof(iface->ipaddr)-1);
	} else if (!strcmp("SN_ADDR", name)) {
		iface = get_iface(IFACE, idx);
		strncpy(iface->snbits, val, sizeof(iface->snbits)-1);
	} else if (!strcmp("IP_SDHCP", name)) {
		iface = get_iface(IFACE, idx);
		strncpy(iface->dhcps, val, sizeof(iface->dhcps)-1);
	} else if (!strcmp("IP_EDHCP", name)) {
		iface = get_iface(IFACE, idx);
		strncpy(iface->dhcpe, val, sizeof(iface->dhcpe)-1);
	} else if (!strcmp("INT_BWIN", name)) {
		iface = get_iface(IFACE, idx);
		strncpy(iface->bwin, val, sizeof(iface->bwin)-1);
	} else if (!strcmp("INT_BWOUT", name)) {
		iface = get_iface(IFACE, idx);
		strncpy(iface->bwout, val, sizeof(iface->bwout)-1);
	} else if (!strcmp("INT_MAC", name)) {
		iface = get_iface(IFACE, idx);
		strncpy(iface->macaddr, val, sizeof(iface->macaddr)-1);
	} else if (!strcmp("INT_GW", name)) {
		iface = get_iface(IFACE, idx);
		strncpy(iface->gateway, val, sizeof(iface->gateway)-1);
	} else if (!strcmp("INT_NAME", name)) {
		iface = get_iface(IFACE, idx);
		strncpy(iface->intname, val, sizeof(iface->intname)-1);
	} else if (!strcmp("INT_IFACE", name)) {
		iface = get_iface(IFACE, idx);
		strncpy(iface->iface, val, sizeof(iface->iface)-1);
	} else if (!strcmp("IP_ADDRV", name)) {
		iface = get_iface(VLAN, idx);
		strncpy(iface->ipaddr, val, sizeof(iface->ipaddr)-1);
	} else if (!strcmp("SN_ADDRV", name)) {
		iface = get_iface(VLAN, idx);
		strncpy(iface->snbits, val, sizeof(iface->snbits)-1);
	} else if (!strcmp("IP_SDHCPV", name)) {
		iface = get_iface(VLAN, idx);
		strncpy(iface->dhcps, val, sizeof(iface->dhcps)-1);
	} else if (!strcmp("IP_EDHCPV", name)) {
		iface = get_iface(VLAN, idx);
		strncpy(iface->dhcpe, val, sizeof(iface->dhcpe)-1);
	} else if (!strcmp("INT_BWINV", name)) {
		iface = get_iface(VLAN, idx);
		strncpy(iface->bwin, val, sizeof(iface->bwin)-1);
	} else if (!strcmp("INT_BWOUTV", name)) {
		iface = get_iface(VLAN, idx);
		strncpy(iface->bwout, val, sizeof(iface->bwout)-1);
	} else if (!strcmp("INT_GWV", name)) {
		iface = get_iface(VLAN, idx);
		strncpy(iface->gateway, val, sizeof(iface->gateway)-1);
	} else if (!strcmp("INT_NAMEV", name)) {
		iface = get_iface(VLAN, idx);
		strncpy(iface->intname, val, sizeof(iface->intname)-1);
	} else if (!strcmp("INT_PARV", name)) {
		iface = get_iface(VLAN, idx);
		strncpy(iface->iface, val, sizeof(iface->iface)-1);
	} else if (!strcmp("VLAN", name)) {
		iface = get_iface(VLAN, idx);
		iface->id = atoi(val);
	} else if (!strcmp("IP_ADDRA", name)) {
		iface = get_iface(ALIAS, idx);
		strncpy(iface->ipaddr, val, sizeof(iface->ipaddr)-1);
	} else if (!strcmp("SN_ADDRA", name)) {
		iface = get_iface(ALIAS, idx);
		strncpy(iface->snbits, val, sizeof(iface->snbits)-1);
	} else if (!strcmp("INT_NAMEA", name)) {
		iface = get_iface(ALIAS, idx);
		strncpy(iface->intname, val, sizeof(iface->intname)-1);
	} else if (!strcmp("ALIAS", name)) {
		iface = get_iface(ALIAS, idx);
		strncpy(iface->iface, val, sizeof(iface->iface)-1);
	}
}

void free_list() {
	struct intface *last=iflst;

	do {
		last = iflst;
		iflst=iflst->next;
		free(last);
	} while(iflst);
}

void remove_iface(struct xml_doc *xmldoc) {
	struct xml_search *xsearch;
	struct xml_node *xn;
	void *iter;

	if ((xsearch = xml_xpath(xmldoc, "/config/IP/Interfaces/Interface", NULL))) {
		for(xn = xml_getfirstnode(xsearch, &iter); xn; xn = xml_getnextnode(iter)) {
			xml_delete(xn);
			objunref(xn);
		}
		objunref(xsearch);
		objunref(iter);
	}
}

void store_iface(struct xml_doc *xmldoc) {
	struct intface *iface = iflst;
	struct xml_node *inode;
	char tmp[16], ifname[24];
	int sn;

	remove_iface(xmldata);

	for(;iface;iface = iface->next) {
		sn = atoi(iface->snbits);
		switch(iface->type) {
			case IFACE:
				inode = xml_addnode(xmldoc, "/config/IP/Interfaces", "Interface", iface->iface, "macaddr", iface->macaddr);
			break;
			case VLAN:
				snprintf(ifname, 23, "%s.%i", iface->iface, iface->id);
				inode = xml_addnode(xmldoc, "/config/IP/Interfaces", "Interface", ifname, "macaddr", "00:00:00:00:00:00");
			break;
			case ALIAS:
				inode = xml_addnode(xmldoc, "/config/IP/Interfaces", "Interface", iface->iface, "macaddr", "00:00:00:00:00:00");
			break;
		};
		if (!inode) {
			continue;
		}

		xml_setattr(xmldoc, inode, "ipaddr", iface->ipaddr);
		xml_setattr(xmldoc, inode, "subnet", iface->snbits);
		xml_setattr(xmldoc, inode, "name", iface->intname);
		xml_setattr(xmldoc, inode, "bwout", iface->bwout);
		xml_setattr(xmldoc, inode, "gateway", iface->gateway);
		xml_setattr(xmldoc, inode, "bwin", iface->bwin);
		xml_setattr(xmldoc, inode, "dhcpend", iface->dhcpe);
		xml_setattr(xmldoc, inode, "dhcpstart", iface->dhcps);
		getnetaddr(iface->ipaddr, sn, tmp, 16);
		xml_setattr(xmldoc, inode, "nwaddr", tmp);
		cidrtosn(sn, tmp, 16);
		xml_setattr(xmldoc, inode, "netmask", tmp);
		getbcaddr(iface->ipaddr, sn, tmp, 16);
		xml_setattr(xmldoc, inode, "bcaddr", tmp);
		objunref(inode);
	}
}


void update_xml_path(struct xml_doc *xmldoc, char *xpath, char *value) {
	struct xml_search *npath;
	struct xml_node *xn;

	if (!(npath = xml_xpath(xmldoc, xpath, NULL))) {
		return;
	}

	if (!(xn = xml_getfirstnode(npath, NULL))) {
		objunref(npath);
		return;
	}

	if (xml_nodecount(npath) == 1) {
		xml_modify(xmldoc, xn, value);
	}
}

void update_xml_option(struct xml_doc *xmldoc, char *xpath, char *name, char *value) {
	struct xml_search *opath;
	struct xml_node *onode;
	char spath[128];

	snprintf(spath, sizeof(spath)-1, "%s/Option", xpath);

	if (!(opath = xml_xpath(xmldoc, spath, "option"))) {
		return;
	}

	if (!(onode = xml_getnode(opath, name))) {
		if (value && (onode = xml_addnode(xmldoc, xpath, "Option", value, "option", name))) {
			objunref(onode);
		}
		objunref(opath);
		return;
	}

	if (value) {
		xml_modify(xmldoc, onode, value);
	} else {
		xml_delete(onode);
	}

	objunref(opath);
	objunref(onode);

}


void modem_update(struct xml_doc *xmldoc, char *name, char *value) {
	char modopt[64];

	if (!strcmp("MDM_PORT", name)) {
		strncpy(modopt, "ComPort", sizeof(modopt)-1);
	} else if (!strcmp("MDM_SPEED", name)) {
		strncpy(modopt, "Speed", sizeof(modopt)-1);
	} else if (!strcmp("MDM_FLW", name)) {
		strncpy(modopt, "FlowControl", sizeof(modopt)-1);
	} else if (!strcmp("MDM_CONN", name)) {
		strncpy(modopt, "Connection", sizeof(modopt)-1);
	} else if (!strcmp("MDM_Init1", name)) {
		strncpy(modopt, "Init1", sizeof(modopt)-1);
	} else if (!strcmp("MDM_Init2", name)) {
		strncpy(modopt, "Init2", sizeof(modopt)-1);
	} else if (!strcmp("MDM_DSTR", name)) {
		strncpy(modopt, "DialString", sizeof(modopt)-1);
	} else if (!strcmp("MDM_MTU", name)) {
		strncpy(modopt, "MTU", sizeof(modopt)-1);
	} else if (!strcmp("MDM_ADDR", name)) {
		strncpy(modopt, "Address", sizeof(modopt)-1);
	} else if (!strcmp("MDM_GW", name)) {
		strncpy(modopt, "Gateway", sizeof(modopt)-1);
	} else if (!strcmp("MDM_TOUT", name)) {
		strncpy(modopt, "IdleTimeout", sizeof(modopt)-1);
	} else if (!strcmp("MDM_HO", name)) {
		strncpy(modopt, "Holdoff", sizeof(modopt)-1);
	} else if (!strcmp("MDM_MF", name)) {
		strncpy(modopt, "Maxfail", sizeof(modopt)-1);
	} else if (!strcmp("MDM_NUM", name)) {
		strncpy(modopt, "Number", sizeof(modopt)-1);
	} else if (!strcmp("MDM_UN", name)) {
		strncpy(modopt, "Username", sizeof(modopt)-1);
	} else if (!strcmp("MDM_PW", name)) {
		strncpy(modopt, "Password", sizeof(modopt)-1);
	} else {
		return;
	}

	update_xml_option(xmldoc, "/config/IP/Dialup", modopt, value);
}

void voip_update(struct xml_doc *xmldoc, char *name, char *value) {
	struct xml_search *vpath;
	struct xml_node *vnode;
	char vopt[64];

	if (!strcmp("LCRAC", name)) {
		strncpy(vopt, "username", sizeof(vopt)-1);
	} else if (!strcmp("LCRDTMF", name)) {
		strncpy(vopt, "dtmf", sizeof(vopt)-1);
	} else if (!strcmp("LCRFROMU", name)) {
		strncpy(vopt, "fromuser", sizeof(vopt)-1);
	} else if (!strcmp("LCRGWID", name)) {
		strncpy(vopt, "gkid", sizeof(vopt)-1);
	} else if (!strcmp("LCRH323P", name)) {
		strncpy(vopt, "prefix", sizeof(vopt)-1);
	} else if (!strcmp("LCRPROTO", name)) {
		strncpy(vopt, "protocol", sizeof(vopt)-1);
	} else if (!strcmp("LCRPW", name)) {
		strncpy(vopt, "secret", sizeof(vopt)-1);
	} else if (!strcmp("LCRREG", name)) {
		strncpy(vopt, "register", sizeof(vopt)-1);
	} else if (!strcmp("LCRSRTP", name)) {
		strncpy(vopt, "srtp", sizeof(vopt)-1);
	} else if (!strcmp("LCRSRV", name)) {
		strncpy(vopt, "server", sizeof(vopt)-1);
	} else if (!strcmp("LCRVIDEO", name)) {
		strncpy(vopt, "novideo", sizeof(vopt)-1);
	}

	if (!(vpath = xml_xpath(xmldoc, "/config/IP/VOIP", NULL))) {
		if ((vnode = xml_addnode(xmldoc, "/config/IP", "VOIP", NULL, vopt, value))) {
			objunref(vnode);
		}
		return;
	}

	if (!(vnode = xml_getfirstnode(vpath, NULL))) {
		objunref(vpath);
		return;
	}

	xml_setattr(xmldoc, vnode, vopt, value);

	objunref(vnode);
	objunref(vpath);
}

void x509_update(struct xml_doc *xmldoc, char *name, char *value) {
	struct xml_search *xpath;
	struct xml_node *xnode;
	char xopt[64];

	if (!strcmp("X509_C", name)) {
		strncpy(xopt, "Country", sizeof(xopt)-1);
	} else if (!strcmp("X509_ST", name)) {
		strncpy(xopt, "State", sizeof(xopt)-1);
	} else if (!strcmp("X509_L", name)) {
		strncpy(xopt, "City", sizeof(xopt)-1);
	} else if (!strcmp("X509_O", name)) {
		strncpy(xopt, "Company", sizeof(xopt)-1);
	} else if (!strcmp("X509_OU", name)) {
		strncpy(xopt, "Division", sizeof(xopt)-1);
	} else if (!strcmp("X509_CN", name)) {
		strncpy(xopt, "Name", sizeof(xopt)-1);
	} else if (!strcmp("X509_EMAIL", name)) {
		strncpy(xopt, "Email", sizeof(xopt)-1);
	}

	update_xml_option(xmldoc, "/config/X509", xopt, value);
}

void sysconf_update(struct xml_doc *xmldoc, char *name, char *value) {
	update_xml_option(xmldoc, "/config/IP/SysConf", name, value);
}

void dnsconf_update(struct xml_doc *xmldoc, char *name, char *value) {
	update_xml_option(xmldoc, "/config/DNS/Config", name, value);
}

void emailconf_update(struct xml_doc *xmldoc, char *name, char *value) {
	update_xml_option(xmldoc, "/config/Email/Config", name, value);
}

void handle_options(struct xml_doc *xmldoc, char *name, char *value) {
	char tmp[128];

	if (!strncmp("MDM", name, 3)) {
		modem_update(xmldoc, name, value);
	} else if (!strncmp("LCR", name, 3)) {
		voip_update(xmldoc, name, value);
	} else if (!strncmp("X509", name, 3)) {
		x509_update(xmldoc, name, value);
	} else if (!strcmp(name, "DNS_SERV1")) {
		sysconf_update(xmldoc, "PrimaryDns", value);
	} else if (!strcmp(name, "DNS_SERV2")) {
		sysconf_update(xmldoc, "SecondaryDns", value);
	} else if (!strcmp(name, "WINS_SERV1")) {
		sysconf_update(xmldoc, "PrimaryWins", value);
	} else if (!strcmp(name, "WINS_SERV2")) {
		sysconf_update(xmldoc, "SecondaryWins", value);
	} else if (!strcmp(name, "FWALL_EXT")) {
		sysconf_update(xmldoc, "External", value);
	} else if (!strcmp(name, "FWALL_INT")) {
		sysconf_update(xmldoc, "Internal", value);
	} else if (!strcmp(name, "NTP_SERV")) {
		sysconf_update(xmldoc, "NTPServer", value);
	} else if (!strcmp(name, "GW_ADDR")) {
		sysconf_update(xmldoc, "Nexthop", value);
	} else if (!strcmp(name, "DYN_SERV")) {
		dnsconf_update(xmldoc, "DynServ", value);
	} else if (!strcmp(name, "DYN_KEY")) {
		dnsconf_update(xmldoc, "SmartKey", value);
	} else if (!strcmp(name, "HN_ADDR")) {
		dnsconf_update(xmldoc, "Hostname", value);
	} else if (!strcmp(name, "DOM_ADDR")) {
		dnsconf_update(xmldoc, "Domain", value);
	} else if (!strcmp(name, "DYN_ZONE")) {
		dnsconf_update(xmldoc, "DynZone", value);
	} else if (!strcmp(name, "DOM_ADDR")) {
		emailconf_update(xmldoc, "Domain", value);
	} else if (!strcmp(name, "DNS_MX1")) {
		emailconf_update(xmldoc, "MailExchange1", value);
	} else if (!strcmp(name, "DNS_MX2")) {
		emailconf_update(xmldoc, "MailExchange2", value);
	} else if (!strcmp(name, "SMTP_FWD")) {
		emailconf_update(xmldoc, "Smarthost", value);
	} else if (!strcmp(name, "DOM_WG")) {
		update_xml_option(xmldoc, "/config/FileServer/Setup", "Domain", value);
	} else if (!strcmp(name, "DTYPE")) {
		update_xml_option(xmldoc, "/config/FileServer/Setup", "Security", value);
	} else if (!strcmp(name, "DOM_DC")) {
		update_xml_option(xmldoc, "/config/FileServer/Setup", "ADSServer", value);
	} else if (!strcmp(name, "DOM_ADS")) {
		update_xml_option(xmldoc, "/config/FileServer/Setup", "ADSRealm", value);
	} else if (!strcmp(name, "NB_NAME")) {
		snprintf(tmp, sizeof(tmp)-1, "netbios name = %s", value);
		update_xml_path(xmldoc, "/config/FileServer/Config/Item[starts-with(.,'netbios name = ')]", tmp);
	} else if (!strcmp(name, "OSLEVEL")) {
		snprintf(tmp, sizeof(tmp)-1, "os level = %s", value);
		update_xml_path(xmldoc, "/config/FileServer/Config/Item[starts-with(.,'os level = ')]", tmp);
	} else if (strcmp(name, "VLAN_PHY") && strcmp(name, "DEL_DNS")) {
		printf("%s -> %s\n", name, value);
	}

/*
DOMC -> 0
SERIAL -> C35AF2-6A8B71-D812F1-D178DC-EA37DB-1E6898-CD
*/

}

int main(int argc, char **argv) {
	FILE *fp;
	char name[50];
	char buff[BUFSIZ];
	char val[50];
	int idx;
	void *xbuf;

	if ((argc > 1) && (fp=fopen(argv[1],"r"))==NULL) {
		perror("File cannot be opened");
		exit(1);
	}


	startthreads();
	xml_init();
	xslt_init();

	if (!(xmldata = xml_loaddoc(argv[2], 1))) {
		perror("Config failed to load\n");
	}

	while(fgets(buff,sizeof(buff),fp)!=NULL) {
		if(sscanf(buff,"%[^\[][%d]=%*[\"]%[^\"^;]", name, &idx, val) == 3) {
			handle_iface_opts(name, idx, val);
		} else if(sscanf(buff,"%[^\[][%d]=%*[\"]%*[^\"^;]", name, &idx) == 2) {
			handle_iface_opts(name, idx, "");
		} else if(sscanf(buff,"%[^=]%*[=\"]%[^\"^;]", name, val) == 2) {
			handle_options(xmldata, name, val);
		} else if(sscanf(buff,"%[^=]%*[=\"]%*[^\"^;]", name) == 1) {
			handle_options(xmldata, name, NULL);
		} else {
			printf("line cannot be printed %s\n", buff);
		}
	}

	store_iface(xmldata);

	xbuf = xml_doctobuffer(xmldata);
	printf("%s\n",xml_getbuffer(xbuf));
	objunref(xbuf);

	free_list();

	objunref(xmldata);

	xslt_close();
	xml_close();
	stopthreads(1);
	return 0;
}
