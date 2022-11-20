
#include "eprom.h"

 /** Print the value os a json object or array.
  * @param json The handler of the json object or array. */
static void dump( json_t const* json ) {

    jsonType_t const type = json_getType( json );
    if ( type != JSON_OBJ && type != JSON_ARRAY ) {
        puts("error");
        return;
    }

    printf( "%s", type == JSON_OBJ? " {": " [" );

    json_t const* child;
    for( child = json_getChild( json ); child != 0; child = json_getSibling( child ) ) {

        jsonType_t propertyType = json_getType( child );
        char const* name = json_getName( child );
        if ( name ) printf(" \"%s\": ", name );

        if ( propertyType == JSON_OBJ || propertyType == JSON_ARRAY )
            dump( child );

        else {
            char const* value = json_getValue( child );
            if ( value ) {
                bool const text = JSON_TEXT == json_getType( child );
                char const* fmt = text? " \"%s\"": " %s";
                printf( fmt, value );
                bool const last = !json_getSibling( child );
                if ( !last ) putchar(',');
            }
        }
    }

    printf( "%s", type == JSON_OBJ? " }": " ]" );

}


// Store this locally for now, later will come from Android side, probably from a SQLite Database.

    char config_27c512[] = "{\n"
"  \"type\" : \"TMS27PC512\",\n"
"  \"alias\" : [ \"27C512\", \"27C512A\"],\n"
"  \"package\" : \"DIP\",\n"
"  \"pins\":[\"A15\",\"A12\",\"A7\",\"A6\",\"A5\",\"A4\",\"A3\",\"A2\",\"A1\",\"A0\",\"D0\",\"D1\",\"D2\",\"GND\",\"D3\",\"D4\",\"D5\",\"D6\",\"D7\",\"/E\",\"A10\",\"/G\",\"A11\",\"A9\",\"A8\",\"A13\",\"A14\",\"VCC\"],\n"
"   \"gnd\":[\"GND\"],\n"
"    \"power\":[\"VCC\"],  \n"
"    \"access\":[{\"E\":\"LOW\"},\n"
"            {\"G\":\"LOW\"}]\n"
"}";



            //ZIF mapping, remember to adjust for the EPROM
            // Also remember Pin 1 not Pin 0!

#define ZIF_SOCKET_PINCOUNT  48
// Mapping table based on MODIFIED AUER-1-0_2022-04-02 // Should mapping table with new adapter board
static const char *zif_io_mapping[] = { "PD.5",          // Pin 1
                                 "PD.2",             	// Pin 2
                                 "PD.1",                 // Pin 3
                                 "PA.7",			// Pin 4
                                 "PA.6",			// Pin 5
                                 "PA.5",			// Pin 6
                                 "PA.4",			// Pin 7
                                 "PA.3",			// Pin 8
                                 "PA.2",			// Pin 9
                                 "PA.1",			// Pin 10
                                 "PA.0",			// Pin 11
                                 "PB.11",			// Pin 12
                                 //"PC.3",			// Pin 13  WARNING, THis is NOT the same as the schematic! Taken from pin 21 // why we cant use P14 -> pin 13
                                 "PB.10"            // PIN 13
                                 "GND",				// Pin 14 Switched with PIN13
                                 "PC.0",			// Pin 15
                                 "PC.8",			// Pin 16
                                 "PC.1",			// Pin 17
                                 "PC.9",			// Pin 18
                                 "PC.2",			// Pin 19
                                 "PC.10",			// Pin 20
                                 //"NC",  			// Pin 21 // why
                                 "PC.3"
                                 "PC.11",			// Pin 22
                                 "X2",				// Pin 23
                                 "X4",				// Pin 24
                                 "X5",				// Pin 25
                                 "X3",				// Pin 26
                                 "VCC",	    		// Pin 27
                                 "PC.4",			// Pin 28
                                 "PC.12",			// Pin 29
                                 "PC.5",			// Pin 30
                                 "PC.13",			// Pin 31
                                 "PC.6",			// Pin 32
                                 "PC.14",			// Pin 33
                                 "PC.7",			// Pin 34
                                 "PC.15",			// Pin 35
                                 "PC.4", 			// Pin 36
                                 "PB.9",			// Pin 37
                                 "PD.0",			// Pin 38
                                 "PA.15",           // Pin 39
                                 "PA.14",           // Pin 40
                                 "PA.13",           // Pin 41
                                 "PA.12",           // Pin 42
                                 "PA.11",           // Pin 43
                                 "PA.10",           // Pin 44
                                 "PA.9",            // Pin 45
                                 "PA.8",            // Pin 46
                                 "PD.3",            // Pin 47
                                 "VCC",             // Pin 48


 };
#if 0
static const char *dil28_io_mapping_adapter_board[] = // this map will is in real DIL adapter board
{
    "PD.5",  // Pin 1
    "PD.2",  // Pin 2
    "PD.1",  // Pin 3
    "PA.7",  // Pin 4
    "PA.6",  // Pin 5
    "PA.5",  // Pin 6
    "PA.4",  // Pin 7
    "PA.3",  // Pin 8
    "PA.2",  // Pin 9
    "PA.1",  // Pin 10
    "PA.0",  // Pin 11
    "PB.11", // Pin 12
    "PB.10", // PIN 13
    "GND",   // Pin 14
    "PC.0",  // Pin 15
    "PC.8",  // Pin 16
    "PC.1",  // Pin 17
    "PC.9",  // Pin 18
    "PC.2",  // Pin 19
    "PC.3",  // Pin 20
    "PC.11", // Pin 21
    "PC.4",  // Pin 22
    "PC.12", // Pin 23
    "PC.5",  // Pin 24
    "PC.13", // Pin 25
    "PC.14", // Pin 26
    "PC.15", // Pin 27
    "VCC",   // Pin 28

};
#else
static const char *dil28_io_mapping_adapter_board[] = // this map will is in real DIL adapter board
{
    "PA.0",  // Pin 1
    "PA.1",  // Pin 2
    "PA.2",  // Pin 3
    "PA.3",  // Pin 4
    "PA.4",  // Pin 5
    "PA.5",  // Pin 6
    "PA.6",  // Pin 7
    "PA.7",  // Pin 8
    "PA.8",  // Pin 9
    "PA.9",  // Pin 10
    "PC.15",  // Pin 11
    "PC.14", // Pin 12
    "PC.13", // PIN 13
    "GND",   // Pin 14
    "PC.12",  // Pin 15
    "PC.11",  // Pin 16
    "PC.10",  // Pin 17
    "PC.9",  // Pin 18
    "PC.8",  // Pin 19
    "PB.3",  // Pin 20
    "PA.11", // Pin 21
    "PD.5",  // Pin 22
    "PA.12", // Pin 23
    "PA.13", // Pin 24
    "PA.10", // Pin 25
    "PA.14", // Pin 26
    "PA.15", // Pin 27
    "VCC",   // Pin 28
};
#endif

static const char *dil32_io_mapping_adapter_board[] = // this map will is in real DIL adapter board
{
    "PA.0",  // Pin 1
    "PA.1",  // Pin 2
    "PA.2",  // Pin 3
    "PA.3",  // Pin 4
    "PA.4",  // Pin 5
    "PA.5",  // Pin 6
    "PA.6",  // Pin 7
    "PA.7",  // Pin 8
    "PA.8",  // Pin 9
    "PA.9",  // Pin 10
    "PC.15",  // Pin 11
    "PC.14", // Pin 12
    "PC.13", // PIN 13
    "PB.13", // PIN 14
    "PB.14", // PIN 15
    "GND",   // Pin 16
    "PC.12",  // Pin 17
    "PC.11",  // Pin 18
    "PC.10",  // Pin 19
    "PC.9",  // Pin 20
    "PC.8",  // Pin 21
    "PC.8",  // Pin 22
    "PB.3",  // Pin 23
    "PA.11", // Pin 24
    "PD.5",  // Pin 25
    "PD.5",  // Pin 26
    "PA.12", // Pin 27
    "PA.13", // Pin 28
    "PA.10", // Pin 29
    "PA.14", // Pin 30
    "PA.15", // Pin 31
    "VCC",   // Pin 32
};

#define SIZE_OF_ARRAY_DIL28 (sizeof(dil28_io_mapping_adapter_board)/sizeof(dil28_io_mapping_adapter_board[0]))
#define SIZE_OF_ARRAY_DIL32 (sizeof(dil28_io_mapping_adapter_board)/sizeof(dil28_io_mapping_adapter_board[0]))


int count_pins(json_t *json)
{
    int count = 1;
    json_t const *pins = json_getProperty(json, "pins");
    if (!pins || JSON_ARRAY != json_getType(pins))
    {
        printf("ERROR: pins is not an array!\n");
    }
    json_t const *pin;
    for (pin = json_getChild(pins); pin != 0; pin = json_getSibling(pin))
    {
        if (JSON_TEXT == json_getType(pin))
        {
	        char const* pinname = json_getValue( pin );
	        //printf ("Pinname %d = %s\n",count++,pinname);
          count++;
	    }
   }
   count--;
   //printf ("\nTotal Pincount = %d\n",count);
   return count;
}

static pin_types_t check_state_pin(const char *pinname)
{
  int a, b;
	a=(int)pinname[0];
	a=toupper((int)pinname[0]);        // Ignore the P and get the port after
	b=toupper((int)pinname[1]);        // Ignore the P and get the port after

    if (a == 'A') // A15, A12
    {
       return ADDR_PIN;
    }
    else if (a == 'D') // D0, D1
    {
        return DATA_PIN;
    }
    else if((a == 'V') || (a == 'G')) // VCC, GND
    {
        return POWER_PIN;
    }
    else
    {
        if (b == 'E' || b == 'C') // \E or CE pins
        {
          return CHIP_ENABLE_PIN;
        }
        else if (b == 'O' || b == 'G') // others type can have OE pin
        {
          return OUTPUT_ENABLE_PIN;
        }
        else
        {
          return OTHERS_PIN;
        }

    }
    // @todo: Should have more type pins here, WE CAN'T CONTROL ALL THE TYPE.
}

static void get_index_state(json_t *json, uint8_t *index)
{
  int count = 0;
  json_t const *pins = json_getProperty(json, "pins");
  if (!pins || JSON_ARRAY != json_getType(pins))
  {
    printf("ERROR: pins is not an array!\n");
  }
  json_t const *pin;
  for (pin = json_getChild(pins); pin != 0; pin = json_getSibling(pin))
  {
    if (JSON_TEXT == json_getType(pin))
    {
      char const* pinname = json_getValue(pin);
      index[count] = check_state_pin(pinname);
      count++;
    }
  }
  return count;
}
static void drive_OE_pin(int fd, const char **array, const uint8_t * pin_types, int length, bool value) //assert_read(* access_array_object
{
  uint32_t port_mask[PORT_DEF_SIZE] = {0};

  uint32_t port;
  uint32_t bit;
  for (int a = 0; a < length; a++)
  {
    if (pin_types[a] == OUTPUT_ENABLE_PIN)
    {
      port = getport(array[a]);
      bit = getportbitnum(array[a]);
      port_mask[port] |= NUC_BIT(bit); // All data bits are going to be in the mask
    }
  }
  for (int a = (int)PORTA; a <= (int)PORTF; a++)
  {
    if (port_mask[a] != 0)
    {
      //D_PRINTF("Drive OE pin- Port %8.8X, direction mask %8.8X, Value %8.8X\n", NUC_PORT_A_BASE + a * 0x40 + 8, port_mask[a], value); // Should printf the real DOUT
      if (value)
        factory_cmd_write(fd, NUC_PORT_A_BASE + a * 0x40 + 8, port_mask[a], port_mask[a]);    // Should set all the to low , with the mask for all output port
      else
        factory_cmd_write(fd, NUC_PORT_A_BASE + a * 0x40 + 8, port_mask[a], value);    // Should set all the to low , with the mask for all output port
    }
  }
}


static void set_input (int fd, const char **array, int length, const uint8_t * pin_types)
{
	uint32_t port_direction_array[PORT_DEF_SIZE]= { 0 };
	uint32_t port_direction_mask[PORT_DEF_SIZE]= { 0 };
	uint32_t port;
	uint32_t bit;

	// All input side is set to inputs, not just odd or even
	for (int a=0;a<length;a++)
	{
    if (pin_types[a] == DATA_PIN) // JUST SET INPUT FOR DATA PIN.
    {
  		port=getport(array[a]);
  		bit=getportbitnum(array[a]);
  		port_direction_array[port]|=NUC_DIR(INPUT, bit);
  		port_direction_mask[port]|=NUC_DIR_MASK(bit);
    }
	}

	// Now Write these Data Directions
	for (int a=PORTA;a<=PORTF;a++)
	{
		if (port_direction_mask[a]!=0)
		{
			D_PRINTF("Set Input - Port %8.8X, direction mask %8.8X, Value %8.8X\n", NUC_PORT_A_BASE+a*0x40+0,  port_direction_mask[a], port_direction_array[a]);
			factory_cmd_write(fd, NUC_PORT_A_BASE+a*0x40+0,  port_direction_mask[a], port_direction_array[a]);  // Remember Mask first, then value
		}
	}
}

static void set_output(int fd, const char **array, int length, const uint8_t * pin_types)
{
	uint32_t port_array[PORT_DEF_SIZE]= { 0 };
	uint32_t port_mask[PORT_DEF_SIZE]= { 0 };
	uint32_t port_direction_array[PORT_DEF_SIZE]= { 0 };
	uint32_t port_direction_mask[PORT_DEF_SIZE]= { 0 };

	uint32_t port;
	uint32_t bit;
	// Set all ports on this 'side' as output parity bits as high and !parity bits as low, set directions to Quasi
	for (int a=0;a<length;a++)
	{
    if ((pin_types[a] == ADDR_PIN) || (pin_types[a] == OUTPUT_ENABLE_PIN) || (pin_types[a] == CHIP_ENABLE_PIN))
    {
      port=getport(array[a]);
		  bit=getportbitnum(array[a]);
		  port_direction_array[port]|=NUC_DIR(QUASI, bit);
		  port_direction_mask[port]|=NUC_DIR_MASK(bit);
		  port_mask[port]|=NUC_BIT(bit);                     // All data bits are going to be in the mask
    }
	}

	for (int a=(int)PORTA;a<=(int)PORTF;a++)
	{
		if (port_mask[a]!=0)
		{
			D_PRINTF("Set Output PMD- Port %8.8X, direction mask %8.8X, Value %8.8X\n", NUC_PORT_A_BASE+a*0x40+0,  port_direction_mask[a], port_direction_array[a]);
			factory_cmd_write(fd, NUC_PORT_A_BASE+a*0x40+0,  port_direction_mask[a], port_direction_array[a]);  // Set Mode to Quasi

			D_PRINTF("Set Output DOUT- Port %8.8X, direction mask %8.8X, Value %8.8X\n", NUC_PORT_A_BASE+a*0x40+8,  port_mask[a], port_array[a]); // Should printf the real DOUT
			factory_cmd_write(fd, NUC_PORT_A_BASE+a*0x40+8,  port_mask[a], 0);   // Should set all the to low , with the mask for all output port
		}
	}

	// factory_cmd_write(fd, NUC_PORT_D_BASE+0x40+0,  0x0C, 3);  // Set Mode to Quasi
	// factory_cmd_write(fd, NUC_PORT_D_BASE+0x40+8,  2, 0);   // Should set all the to low , with the mask for all output port

}

void eprom_init_io(int fd, json_t *json, uint8_t lenght, uint8_t *index_array)
{
  get_index_state(json, index_array);

  set_input(fd, dil28_io_mapping_adapter_board, lenght, index_array);
  set_output(fd, dil28_io_mapping_adapter_board, lenght, index_array);

}

void assert_read(int fd, json_t *json, const char **array, pin_types_t acess_type, bool value) // function just use for some particuraly pin
{

  if ((acess_type != OUTPUT_ENABLE_PIN) && (acess_type != CHIP_ENABLE_PIN))
  {
    D_PRINTF("This function just can drive OE or CE pins, if want to control other pins, need to add more conditob");
    return;
  }

  uint8_t count =0;
  count = count_pins(json);

  uint8_t index[count];
  get_index_state(json, index);
  uint32_t port_mask[PORT_DEF_SIZE] = {0};
  uint32_t port;
  uint32_t bit;
  for (int a = 0; a < count; a++)
  {
    if (index[a] == acess_type)
    {
      port = getport(array[a]);
      bit = getportbitnum(array[a]);
      port_mask[port] |= NUC_BIT(bit); // All data bits are going to be in the mask
    }
  }
  for (int a = (int)PORTA; a <= (int)PORTF; a++)
  {
    if (port_mask[a] != 0)
    {
      if (value)
        factory_cmd_write(fd, NUC_PORT_A_BASE + a * 0x40 + 8, port_mask[a], port_mask[a]);    // Set that pin with the mask and value same, That pin will be high
      else
        factory_cmd_write(fd, NUC_PORT_A_BASE + a * 0x40 + 8, port_mask[a], value);    // Should set all the to low , with the mask for all output port
    }
  }
}
// Check index for each pin, have to read from D0 to D7
// Example: Data with 0x0000 address is D0 is bit bit7 | D1 is bit 6 | D2 is bit 5 | D3 is bit 4 |D4 is bit 3 | D5 is bit 2 | D6 is bit 1 |D7 is bit 0
//void read_data_algorithm(int fd,  json_t *json, const char **array, uint8_t *pdata)
void read_data_algorithm(int fd,  json_t *json)
{
  uint8_t i;
  // Process json to detect index of Data pin out, will same with address pin
  json_t const* pins = json_getProperty( json, "pins" );
   // json_t const* child = json_getChild( json );



   json_t const *pin = json_getChild(pins);
   pin+=10;
  char const* name = json_getValue( pin );

  if ( name ) printf("%s \n", name );

  // for (int i = 0; i < 8; i++) // How to read data from one pin and combine it to one byte, data come from each pin. 
  // {
  //   factory_cmd_read(data_Pin, byte & 0x80);
  //   byte = byte << 1;
  // }
	// for (i = 0; i < size; i++)
	// 	{
	// 		if (!write_eeprom (startaddress++, *value++))	/* write single byte to EEPROM			*/
	// 			return (0);									/* return error							*/
	// 	}
	// return (1);		

}

void read_one_byte_from_address(int fd, const char **array, uint32_t address, uint8_t *pdata)
{
  // Set address

  // Drive OE and CE

  // read data: How to read data, seems we need to read 

}

int read_eprom(int fd, const char *eprom_type, const char *rom_filename, uint32_t start_override,
                uint32_t length_override)
{
  uint8_t array_store_type[SIZE_OF_ARRAY_DIL28];
  // First lets test out our JSON decoding
  json_t mem[1000];
  puts(config_27c512);

  json_t const *json = json_create(config_27c512, mem, sizeof mem / sizeof *mem);

  dump(json);

  if (!json)
  {
    puts("Error json create.");
    return EXIT_FAILURE;
  }

  json_t const *chiptype = json_getProperty(json, "type");
  if (!chiptype || JSON_TEXT != json_getType(chiptype))
  {
    puts("Error, the type property is not found.");
    return EXIT_FAILURE;
  }

  char const *Type = json_getValue(chiptype);
  printf("Type: %s.\n", Type);

  printf("\n");

  read_data_algorithm(fd, json);
// ================================== Start config and try to read data from here=========================
  eprom_init_io(fd, json, SIZE_OF_ARRAY_DIL28, array_store_type);
  assert_read(fd,json,dil28_io_mapping_adapter_board,OUTPUT_ENABLE_PIN,LOW);

  return 0;
}


/*------------------------------------porting factory code ----------------------- */
static Ports getport(const char *string)
{ // Should ignrore power port like GND and vcc
	int a;
	uint32_t value;
	a=(int)string[1];
	a=toupper((int)string[1]);        // Ignore the P and get the port after
  if ((a >= 'A') && (a <= 'F'))
  {
    value=(a-'A');
    value+=PORTA;      // Makes debugging easier when I break it up
  }

	return value;
}

static uint32_t getportbitnum(const char *string)
{
	uint32_t value;
	value=strtoumax(string+3, NULL, 10);       // Using C99
	return value;
}
