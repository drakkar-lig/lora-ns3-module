/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Mitch Watrous <watrous@u.washington.edu>
 *         To Thanh Hai <tthhai@gmail.com>
 */

#include "lora-mac.h"

namespace ns3 {

NS_OBJECT_ENSURE_REGISTERED (LoraMac);

TypeId LoraMac::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::LoraMac")
    .SetParent<Object> ()
    .SetGroupName ("Lora")
  ;
  return tid;
}

} // namespace ns3
