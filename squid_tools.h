#ifndef SQUID_TOOLS_H
#define SQUID_TOOLS_H

typedef struct LatLon
{
  double lat = 0.0;
  double lon = 0.0;
} LatLon_t;

class Squid_Tools
{

public:
  Squid_Tools(void);

  void setupTime();
  void setupTime(int hour, int day, int month, int year);

  void calc_m_per_deg(double, double, double *, double *);
  void calc_m_per_deg(double, double *, double *);

  int check_EU_op_id(const char *, const char *);
  char luhn36_check(const char *);
  int luhn36_c2i(char);
  char luhn36_i2c(int);

  void haversineDistance(LatLon_t, double, int, LatLon_t *);
  bool haversineAt(LatLon_t,double, double, int, unsigned long, LatLon_t *);

  void generateMAC(uint8_t mac[6]);

private:
  char s[20];
};

#endif
