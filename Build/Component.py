from dataclasses import dataclass, field

from Property import Property

@dataclass
class Component:
    type: str = ""
    contains_asset_reference: bool = field(default=False)
    properties: list[Property] = field(default_factory=list)
