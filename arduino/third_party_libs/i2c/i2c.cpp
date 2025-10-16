#include <i2c.hpp>

void I2C_Init()
{
    // TWBR = ((F_CPU/SCL) -16)/2 = ((16000000/100000) - 16)/2 = (160 - 16)/2 = 144/2 = 72
    TWBR=72; // 100kHz
    TWSR=(0<<TWPS1)|(0<<TWPS0);
}

uint8_t I2C_Start(uint8_t address)
{
    uint8_t status = 0;

    TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT); /* Enable TWI, generate START */

    while(!(TWCR&(1<<TWINT)))
    {
        /* Wait until TWI finish its current job */
        status=TWSR&0xF8;
    }

    if(status!=0x08)
    {
        /* Check weather START transmitted or not? */
        return 0;
    }

    TWDR=address;
    TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI & clear interrupt flag */

    while(!(TWCR&(1<<TWINT)))
    {
        /* Wait until TWI finish its current job */
    }

    status=TWSR&0xF8;		/* Read TWI status register */	

    /* Check for SLA+W transmitted &ack received */
    if(status==0x18){
        return 1;			/* Return 1 to indicate ack received */
    }

    /* Check for SLA+W transmitted &nack received */
    if(status==0x20)
    {
        return 2;			/* Return 2 to indicate nack received */
    }
    else
    {
        return 3;			/* Else return 3 to indicate SLA+W failed */
    }
}

uint8_t I2C_Repeated_Start(uint16_t address)
{
    uint8_t status;		/* Declare variable */
    TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);/* Enable TWI, generate start */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    status=TWSR&0xF8;		/* Read TWI status register */
    if(status!=0x10)		/* Check for repeated start transmitted */
    return 0;			/* Return 0 for repeated start condition fail */
    TWDR=address;		/* Write SLA+R in TWI data register */
    TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    status=TWSR&0xF8;		/* Read TWI status register */
    if(status==0x40)		/* Check for SLA+R transmitted &ack received */
    return 1;			/* Return 1 to indicate ack received */
    if(status==0x48)		/* Check for SLA+R transmitted &nack received */
    return 2;			/* Return 2 to indicate nack received */
    else
    return 3;			/* Else return 3 to indicate SLA+W failed */
}

uint8_t I2C_Write(uint8_t data)
{
    uint8_t status;		/* Declare variable */
    TWDR=data;			/* Copy data in TWI data register */
    TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    status=TWSR&0xF8;		/* Read TWI status register */
    if(status==0x28)		/* Check for data transmitted &ack received */
    return 0;			/* Return 0 to indicate ack received */
    if(status==0x30)		/* Check for data transmitted &nack received */
    return 1;			/* Return 1 to indicate nack received */
    else
    return 2;			/* Else return 2 for data transmission failure */
}

char I2C_Read_Ack()
{
    TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA); /* Enable TWI, generation of ack */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    return TWDR;
}

char I2C_Read_Nack()
{
    TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    return TWDR;
}

void I2C_Stop()
{
    TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);/* Enable TWI, generate stop */
    while(TWCR&(1<<TWSTO));	/* Wait until stop condition execution */
}


