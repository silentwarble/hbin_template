LINK_BASE = 0x0000;

ENTRY( Start )

SECTIONS
{
    . = LINK_BASE;
    .text : {
        . = LINK_BASE;
        *( .text$STUB );
        *( .text$CODE );
        *( .rdata* );
        *( .global );
        *( .text$E );
        *( .text$P );
    }

    .eh_frame : {
        *( .eh_frame )
    }
}